// Copyright Epic Games, Inc. All Rights Reserved.

#include "SnapOutOfItCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/SphereComponent.h"
#include "Tobias/Combat/CombatData.h"
#include "Tobias/Combat/CombatEntity.h"
#include "Tobias/Combat/CombatGameMode.h"
#include "Tobias/Combat/CombatManager.h"
#include "Emil/Arm.h"
#include "Emil/UI/LevelHUD.h"
#include "Objects/Tobias/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Emil/UI/PausedMenu.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASnapOutOfItCharacter

ASnapOutOfItCharacter::ASnapOutOfItCharacter()
{
#pragma region Character Movement
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
#pragma endregion

	SphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("Perception Trigger"));
	SphereColl->SetSphereRadius(90);
	SphereColl->SetRelativeLocation(FVector(0, 0, 90));
	SphereColl->SetCollisionProfileName(TEXT("Pawn"));
	//SphereColl->SetHiddenInGame(false);
	SphereColl->OnComponentBeginOverlap.AddDynamic(this, &ASnapOutOfItCharacter::OnCollision);
	SphereColl->SetupAttachment(GetMesh());

#pragma region Code from Tobias
	GetMesh()->SetRelativeLocation(MESH_GROUND_OFFSET);
	GetMesh()->SetRelativeRotation(MESH_FACE_DIR_OFFSET);
#pragma endregion

#pragma region Shaynas Code
	auto worldDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(*WORLD_DATA_TABLE_PATH);
	if (worldDataTable.Succeeded()) WorldDataTable = worldDataTable.Object;

	auto characterDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(*CHARACTER_DATA_TABLE_PATH);
	if (characterDataTable.Succeeded()) CharacterDataTable = characterDataTable.Object;

	auto armDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(*ARM_DATA_TABLE_PATH);
	if (armDataTable.Succeeded()) ArmDataTable = armDataTable.Object;
#pragma endregion

#pragma region Code from Emil
	PickUpArmLeft = ConstructorHelpers::FObjectFinder<UInputAction>(*LEFT_PICK_UP_INPUT_ACTION_PATH).Object;
	PickUpArmRight = ConstructorHelpers::FObjectFinder<UInputAction>(*RIGHT_PICK_UP_INPUT_ACTION_PATH).Object;
	Pause = ConstructorHelpers::FObjectFinder<UInputAction>(*PAUSE_INPUT_ACTION_PATH).Object;

	//Melted Arms
	LeftArmMeshComp = InitialiseArm(LEFT_ARM_COMPONENT_NAME);
	RightArmMeshComp = InitialiseArm(RIGHT_ARM_COMPONENT_NAME);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
#pragma endregion
}

void ASnapOutOfItCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

#pragma region Emil´s code
	if (!GetWorld()) return;

	if (auto hud = GetWorld()->GetFirstPlayerController()->GetHUD(); hud->IsA<ALevelHUD>())
		LevelHud = Cast<ALevelHUD>(hud);
#pragma endregion

	auto levelName = GetWorld()->GetMapName();

#pragma region Code from Tobias
	ACombatEntity::SetType(EEntityType::ET_PLAYER);
	ACombatEntity::SetUpEntity();

	LoadActiveArms();

	if (GetWorld()->GetAuthGameMode()->IsA<ACombatGameMode>())
	{
		GetManager()->SetPlayer(this);

		LoadCollectedArm();

		if (GetStats().IdleCombatAnimSequ)
		{
			GetMesh()->PlayAnimation(GetStats().IdleCombatAnimSequ.Get(), true);
			PlayLeftArmAnimation(LeftArmData.ArmIdleLeftAnim.Get(), true);
			PlayRightArmAnimation(RightArmData.ArmIdleRightAnim.Get(), true);
		}

		FollowCamera->AddAdditiveOffset(FTransform(FRotator(2.600000f, -14.400000f, 0.0f),
		                                           FVector(-199.052336 + 400.0, 126.442118, 30.154857),
		                                           FVector(1, 1, 1)), 0.0f);
	}
	else SetPlayerWorldPosition();
#pragma endregion
}

void ASnapOutOfItCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Input
void ASnapOutOfItCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Pickup
		//EnhancedInputComponent->BindAction(,)

		//Emil Start
		//Pick Up
		EnhancedInputComponent->BindAction(PickUpArmLeft, ETriggerEvent::Started, this,
		                                   &ASnapOutOfItCharacter::PickUpLeft);
		//EnhancedInputComponent->BindAction(LeftPickUp, ETriggerEvent::Completed, this, &ASnapOutOfItCharacter::StopPickUpLeft);
		EnhancedInputComponent->BindAction(PickUpArmRight, ETriggerEvent::Started, this,
		                                   &ASnapOutOfItCharacter::PickUpRight);
		//EnhancedInputComponent->BindAction(RightPickUp, ETriggerEvent::Completed, this, &ASnapOutOfItCharacter::PickUpRight);

		EnhancedInputComponent->BindAction(Pause, ETriggerEvent::Started, this, &ASnapOutOfItCharacter::PauseMenu);
		//Emil Stop


		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASnapOutOfItCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASnapOutOfItCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ASnapOutOfItCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASnapOutOfItCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
#pragma endregion

void ASnapOutOfItCharacter::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
#pragma region Code from Tobias
	if (OtherActor == this) return;

	if (OtherActor->IsA<ACombatEntity>())
	{
		GetManager()->SetEncounter(Cast<ACombatEntity>(OtherActor)->GetType());
	}
#pragma endregion
}

#pragma region code from Emil
void ASnapOutOfItCharacter::PauseMenu()
{
	auto state = LevelHud->GetPausedMenu()->GetVisibility();
	if (state == ESlateVisibility::Hidden)
	{
		//Show paused screen
		LevelHud->ShowPausedMenu();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("is paused"));

	}
	else if (state == ESlateVisibility::Visible)
	{
		//Hide paused sreen
		LevelHud->HidePausedMenu();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("is not paused"));

	}
}

void ASnapOutOfItCharacter::SaveArmInInventory(const FArmData& armData)
{
	for (auto element : CharacterData.CollectedArms)
	{
		if (element.ArmType == armData.ArmType) return;
	}
	CharacterData.CollectedArms.Add(FArmInventorySlotData{armData.ArmType, armData.ArmName});

	CharacterDataTable->AddRow(FName(TEXT("GrapplingData")), CharacterData);
}

void ASnapOutOfItCharacter::PickUpArm(const FArmData& armData)
{
	if (armData.ArmType == EArmType::ET_DEFAULT ||
		armData.ArmType == CharacterData.ActiveArmLeft ||
		armData.ArmType == CharacterData.ActiveArmRight)
		return;

	for (int32 i = 0; i < CharacterData.CollectedArms.Num(); i++)
	{
		if (CharacterData.CollectedArms[i].ArmType == armData.ArmType)
		{
			InventorySwapIdx = i;

			SwapArms();

			return;
		}
	}
	SetArmData(armData);
}

bool ASnapOutOfItCharacter::HasEnoughCharges()
{
	switch (AttackSelectionType)
	{
	case ESelectionType::ST_LEFT_ARM:
		return LeftArmData.ArmCharges <= ArmCharges;
	case ESelectionType::ST_RIGHT_ARM:
		return RightArmData.ArmCharges <= ArmCharges;
	default: return false;
	}
}

void ASnapOutOfItCharacter::SetAttackSelectionType(const ESelectionType& selectionType)
{
	AttackSelectionType = selectionType;
}

void ASnapOutOfItCharacter::SetArmData(const FArmData& armData)
{
	if (armData.ArmType == EArmType::ET_DEFAULT) return;

	switch (EquipSelectionType)
	{
	case ESelectionType::ST_LEFT_ARM:
		SaveArmInInventory(LeftArmData);

		LeftArmData = armData;
		if (LeftArmMeshComp) LeftArmMeshComp->SetSkeletalMesh(LeftArmData.LeftArmSkeletalMesh.Get());

		CharacterData.ActiveArmLeft = LeftArmData.ArmType;
		break;
	case ESelectionType::ST_RIGHT_ARM:
		SaveArmInInventory(RightArmData);

		RightArmData = armData;
		if (RightArmMeshComp) RightArmMeshComp->SetSkeletalMesh(RightArmData.RightArmSkeletalMesh.Get());

		CharacterData.ActiveArmRight = RightArmData.ArmType;
		break;
	default: ;
	}
	CharacterDataTable->AddRow(FName(TEXT("GrapplingData")), CharacterData);

	OnArmSwap.Broadcast();
}

void ASnapOutOfItCharacter::SetArmCharges(const uint8& armCharges)
{
	if (ArmCharges = armCharges; ArmCharges >= MAX_ARM_CHARGES) ArmCharges = MAX_ARM_CHARGES;
	OnArmChargeChange.Broadcast(ArmCharges);
}

USkeletalMeshComponent* ASnapOutOfItCharacter::InitialiseArm(const FString& armComponentName)
{
	auto arm = CreateDefaultSubobject<USkeletalMeshComponent>(*armComponentName);
	arm->SetupAttachment(GetMesh());
	arm->SetGenerateOverlapEvents(false);
	arm->SetCanEverAffectNavigation(false);

	return arm;
}

void ASnapOutOfItCharacter::PickUpLeft()
{
	if (!ArmInRange) return;

	EquipSelectionType = ESelectionType::ST_LEFT_ARM;

	ArmInRange->OnArmPickUpSignature.AddDynamic(this, &ASnapOutOfItCharacter::PickUpArm);
	ArmInRange->PickUp(this);

	SetArmNotInRange();
	// 	ArmInRange->Destroy();
	// }
}

void ASnapOutOfItCharacter::PickUpRight()
{
	if (!ArmInRange) return;

	EquipSelectionType = ESelectionType::ST_RIGHT_ARM;

	ArmInRange->OnArmPickUpSignature.AddDynamic(this, &ASnapOutOfItCharacter::PickUpArm);
	ArmInRange->PickUp(this);

	SetArmNotInRange();
	// 	ArmInRange->Destroy();
	// }
}
#pragma endregion

#pragma region code from Tobias
void ASnapOutOfItCharacter::UseHealFlask()
{
	GetMesh()->PlayAnimation(GetStats().HealAnimSequ.Get(), false);

	PlayLeftArmAnimation(LeftArmData.ArmHealLeftAnim.Get(), false);
	PlayRightArmAnimation(RightArmData.ArmHealRightAnim.Get(), false);
}

void ASnapOutOfItCharacter::UpdateAnimInstance(USkeletalMeshComponent* armMesh, UAnimInstance* animInstance)
{
	armMesh->SetAnimInstanceClass(animInstance->GetClass());
}

void ASnapOutOfItCharacter::AttackTarget()
{
	Super::AttackTarget();

	if (CheckParalyseEffect()) return;

	switch (AttackSelectionType)
	{
	case ESelectionType::ST_LEFT_ARM:
		if (GetStats().AttackLeftAnimSequ && LeftArmData.ArmAttackActiveLeftAnim)
		{
			GetMesh()->PlayAnimation(GetStats().AttackLeftAnimSequ.Get(), false);

			PlayLeftArmAnimation(LeftArmData.ArmAttackActiveLeftAnim.Get(), false);
			PlayRightArmAnimation(RightArmData.ArmAttackPassiveRightAnim.Get(), false);
		}
		else ApplyDamageToTarget();

		AttackSelectionType = ESelectionType::ST_RIGHT_ARM;
		break;
	case ESelectionType::ST_RIGHT_ARM:
		if (GetStats().AttackRightAnimSequ && RightArmData.ArmAttackActiveRightAnim)
		{
			GetMesh()->PlayAnimation(GetStats().AttackRightAnimSequ.Get(), false);

			PlayLeftArmAnimation(LeftArmData.ArmAttackPassiveLeftAnim.Get(), false);
			PlayRightArmAnimation(RightArmData.ArmAttackActiveRightAnim.Get(), false);
		}
		else ApplyDamageToTarget();

		AttackSelectionType = ESelectionType::ST_LEFT_ARM;
		break;
	default: ;
	}
	if (ArmCharges++; ArmCharges >= MAX_ARM_CHARGES) ArmCharges = MAX_ARM_CHARGES;
	OnArmChargeChange.Broadcast(ArmCharges);
}

void ASnapOutOfItCharacter::SpecialAttackTarget()
{
	if (CharacterData.ActiveArmRight == EArmType::ET_DEFAULT) return;

	if (CheckParalyseEffect()) return;

	switch (AttackSelectionType)
	{
	case ESelectionType::ST_LEFT_ARM:
		if (GetStats().SpecialLeftAnimSequ && LeftArmData.ArmSpecialAttackActiveLeftAnim)
		{
			GetMesh()->PlayAnimation(GetStats().SpecialLeftAnimSequ.Get(), false);

			PlayLeftArmAnimation(LeftArmData.ArmSpecialAttackActiveLeftAnim.Get(), false);
			PlayRightArmAnimation(RightArmData.ArmSpecialAttackPassiveRightAnim.Get(), false);
		}
		else ApplyDamageToTarget();
		break;
	case ESelectionType::ST_RIGHT_ARM:
		if (GetStats().SpecialRightAnimSequ && RightArmData.ArmSpecialAttackActiveRightAnim)
		{
			GetMesh()->PlayAnimation(GetStats().SpecialRightAnimSequ.Get(), false);

			PlayLeftArmAnimation(LeftArmData.ArmSpecialAttackPassiveLeftAnim.Get(), false);
			PlayRightArmAnimation(RightArmData.ArmSpecialAttackActiveRightAnim.Get(), false);
		}
		else ApplyDamageToTarget();
		break;
	default: ;
	}
}

void ASnapOutOfItCharacter::CheckStatusEffect()
{
	Super::CheckStatusEffect();

	PlayLeftArmAnimation(LeftArmData.ArmIdleLeftAnim.Get(), true);
	PlayRightArmAnimation(RightArmData.ArmIdleRightAnim.Get(), true);
}

void ASnapOutOfItCharacter::ReceiveDamage(const int32& a_damage)
{
	Super::ReceiveDamage(a_damage);

	PlayLeftArmAnimation(LeftArmData.ArmHitLeftAnim.Get(), false);
	PlayRightArmAnimation(RightArmData.ArmHitRightAnim.Get(), false);
}

void ASnapOutOfItCharacter::HandleDeath()
{
	Super::HandleDeath();
}

bool ASnapOutOfItCharacter::CheckDeathConditions()
{
	if (Super::CheckDeathConditions())
	{
		PlayLeftArmAnimation(LeftArmData.ArmDeathLeftAnim.Get(), false);
		PlayRightArmAnimation(RightArmData.ArmDeathRightAnim.Get(), false);

		return true;
	}
	return false;
}

void ASnapOutOfItCharacter::LoadCollectedArm()
{
	CharacterData = *CharacterDataTable->FindRow<FCharacterData>(FName(TEXT("GrapplingData")), "");

	GetManager()->SetArmInUI();
}

void ASnapOutOfItCharacter::LoadActiveArms()
{
	if (auto data = CharacterDataTable->FindRow<FCharacterData>(FName(TEXT("GrapplingData")), ""); data)
		CharacterData = *data;

	LeftArmData.ArmType = CharacterData.ActiveArmLeft;
	RightArmData.ArmType = CharacterData.ActiveArmRight;

	SetArmDataFromTable(LeftArmData.ArmType, ESelectionType::ST_LEFT_ARM);
	SetArmDataFromTable(RightArmData.ArmType, ESelectionType::ST_RIGHT_ARM);
}

void ASnapOutOfItCharacter::SetArmDataFromTable(const EArmType armType, const ESelectionType& armSideSelection)
{
	auto tableRowName = FName(UEnum::GetDisplayValueAsText(armType).ToString());

	if (auto data = ArmDataTable->FindRow<FArmData>(tableRowName, ""); data)
	{
		switch (armSideSelection)
		{
		case ESelectionType::ST_LEFT_ARM:
			LeftArmData = *data;
			if (LeftArmMeshComp) LeftArmMeshComp->SetSkeletalMesh(LeftArmData.LeftArmSkeletalMesh.Get());

			if (GetManager()) GetManager()->SetArmIconInUI(LeftArmData.ArmIcon.Get(), ESelectionType::ST_LEFT_ARM);

			CharacterData.ActiveArmLeft = LeftArmData.ArmType;

			break;
		case ESelectionType::ST_RIGHT_ARM:
			RightArmData = *data;
			if (RightArmMeshComp) RightArmMeshComp->SetSkeletalMesh(RightArmData.RightArmSkeletalMesh.Get());

			if (GetManager()) GetManager()->SetArmIconInUI(RightArmData.ArmIcon.Get(), ESelectionType::ST_RIGHT_ARM);

			CharacterData.ActiveArmRight = RightArmData.ArmType;

			break;
		default: ;
		}
		CharacterDataTable->AddRow(FName(TEXT("GrapplingData")), CharacterData);
	}
}

void ASnapOutOfItCharacter::HandleArmSwitchClicked(const uint8& inventoryIdx, const ESelectionType& armSideSelection)
{
	EquipSelectionType = armSideSelection;
	InventorySwapIdx = inventoryIdx;

	switch (EquipSelectionType)
	{
	case ESelectionType::ST_LEFT_ARM:
		GetMesh()->PlayAnimation(GetStats().ArmSwitchLeftAnimSequ.Get(), false);

		PlayLeftArmAnimation(LeftArmData.ArmSwitchLeftLeftAnim1.Get(), false);
		PlayRightArmAnimation(RightArmData.ArmSwitchLeftRightAnim.Get(), false);
		break;
	case ESelectionType::ST_RIGHT_ARM:
		GetMesh()->PlayAnimation(GetStats().ArmSwitchRightAnimSequ.Get(), false);

		PlayLeftArmAnimation(LeftArmData.ArmSwitchRightLeftAnim.Get(), false);
		PlayRightArmAnimation(RightArmData.ArmSwitchRightRightAnim1.Get(), false);
		break;
	default: ;
	}
}

void ASnapOutOfItCharacter::FinishArmSwitch()
{
	GetMesh()->PlayAnimation(GetStats().IdleCombatAnimSequ.Get(), true);

	PlayLeftArmAnimation(LeftArmData.ArmIdleLeftAnim.Get(), true);
	PlayRightArmAnimation(RightArmData.ArmIdleRightAnim.Get(), true);
}

void ASnapOutOfItCharacter::SwapArms()
{
	auto armType = CharacterData.CollectedArms[InventorySwapIdx].ArmType;

	switch (EquipSelectionType)
	{
	case ESelectionType::ST_LEFT_ARM:
		if (LeftArmData.ArmType == EArmType::ET_DEFAULT) CharacterData.CollectedArms.RemoveAt(InventorySwapIdx);
		else
			CharacterData.CollectedArms[InventorySwapIdx] = FArmInventorySlotData{
				LeftArmData.ArmType, LeftArmData.ArmName
			};
		break;
	case ESelectionType::ST_RIGHT_ARM:
		if (RightArmData.ArmType == EArmType::ET_DEFAULT) CharacterData.CollectedArms.RemoveAt(InventorySwapIdx);
		else
			CharacterData.CollectedArms[InventorySwapIdx] = FArmInventorySlotData{
				RightArmData.ArmType, RightArmData.ArmName
			};
		break;
	default: ;
	}
	SetArmDataFromTable(armType, EquipSelectionType);

	switch (EquipSelectionType)
	{
	case ESelectionType::ST_LEFT_ARM:
		PlayLeftArmAnimation(LeftArmData.ArmSwitchLeftLeftAnim2.Get(), false);
		break;
	case ESelectionType::ST_RIGHT_ARM:
		PlayRightArmAnimation(RightArmData.ArmSwitchRightRightAnim2.Get(), false);
		break;
	default: ;
	}

	GetManager()->SetArmInUI();

	OnArmSwap.Broadcast();
}

void ASnapOutOfItCharacter::ClearCollectedArms()
{
	CharacterData.CollectedArms.Empty();
	CharacterData.ActiveArmRight = EArmType::ET_DEFAULT;

	CharacterDataTable->AddRow(FName(TEXT("GrapplingData")), CharacterData);
}

void ASnapOutOfItCharacter::ApplySpecialDamageToTarget()
{
	switch (AttackSelectionType)
	{
	case ESelectionType::ST_LEFT_ARM:
		GetTarget()->ReceiveDamage(CalcSpecialDamageOutput(LeftArmData.SpecialAttackDamage));

		if (ArmCharges -= LeftArmData.ArmCharges; ArmCharges <= 0) ArmCharges = 0;
		OnArmChargeChange.Broadcast(ArmCharges);

		if (LeftArmData.DamageType == E_DAMAGE_TYPE::DT_LIFE_STEAL) HealDamage(EHealType::HT_LIFE_STEAL);
		
		GetTarget()->ApplyStatusEffect(LeftArmData.DamageType);

		AttackSelectionType = ESelectionType::ST_RIGHT_ARM;
		break;
	case ESelectionType::ST_RIGHT_ARM:
		GetTarget()->ReceiveDamage(CalcSpecialDamageOutput(RightArmData.SpecialAttackDamage));

		if (ArmCharges -= RightArmData.ArmCharges; ArmCharges <= 0) ArmCharges = 0;
		OnArmChargeChange.Broadcast(ArmCharges);
		
		if (RightArmData.DamageType == E_DAMAGE_TYPE::DT_LIFE_STEAL) HealDamage(EHealType::HT_LIFE_STEAL);
		
		GetTarget()->ApplyStatusEffect(RightArmData.DamageType);

		AttackSelectionType = ESelectionType::ST_LEFT_ARM;
		break;
	default: ;
	}
}

void ASnapOutOfItCharacter::PlayLeftArmAnimation(UAnimSequence* animationToPlay, const bool& bAnimLoop) const
{
	LeftArmMeshComp->PlayAnimation(animationToPlay, bAnimLoop);
}

void ASnapOutOfItCharacter::PlayRightArmAnimation(UAnimSequence* animationToPlay, const bool& bAnimLoop) const
{
	RightArmMeshComp->PlayAnimation(animationToPlay, bAnimLoop);
}
#pragma endregion

#pragma region Shaynas Code
void ASnapOutOfItCharacter::SaveLastPosition()
{
	if (auto pawn = GWorld->GetFirstPlayerController()->GetPawn(); pawn->IsA<ASnapOutOfItCharacter>())
	{
		if (auto PlayerCharacter = Cast<ASnapOutOfItCharacter>(pawn); PlayerCharacter)
		{
			// save player position
			WorldData.PlayerLocation = PlayerCharacter->GetActorLocation();

			// save player rotation
			WorldData.PlayerRotation = PlayerCharacter->GetActorRotation();

			// save current level
			FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GWorld));
			WorldData.LastLevelName = CurrentLevelName;

			GetManager()->SetLastLevel(WorldData.LastLevelName);

			if (WorldDataTable->GetRowNames().Num() > 0) WorldDataTable->RemoveRow(FName(TEXT("PlayerSave")));
			WorldDataTable->AddRow(FName(TEXT("PlayerSave")), WorldData);
		}
	}
}

void ASnapOutOfItCharacter::SetPlayerWorldPosition()
{
	if (WorldDataTable->GetRowNames().Num() > 0 && GetWorld()->GetMapName() != FName(TEXT("UEDPIE_0_L_Battle_Arena")))
	{
		auto data = *WorldDataTable->FindRow<FWorldData>(FName(TEXT("PlayerSave")), "");
		SetActorLocation(data.PlayerLocation);
		SetActorRotation(data.PlayerRotation);

		WorldDataTable->RemoveRow(FName(TEXT("PlayerSave")));
	}
}

void ASnapOutOfItCharacter::SetGrappleActive(bool bGrappleActive)
{
	CharacterData.bGrappleActive = bGrappleActive;
	CharacterDataTable->AddRow(FName(TEXT("GrapplingData")), CharacterData);
}
#pragma endregion
