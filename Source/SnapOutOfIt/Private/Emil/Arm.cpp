// Fill out your copyright notice in the Description page of Project Settings.


#include "Emil/Arm.h"
#include "Components/SphereComponent.h"

//Construction of the arm
AArm::AArm()
{
	bIsConstructor = true;
	
	//Creation of the collision zone
	PickUpCollision = CreateDefaultSubobject<USphereComponent>(*ArmColisionBoxName);
	PickUpCollision->InitSphereRadius(ArmCollisionRadiusDefault);
	PickUpCollision->SetRelativeLocation(ArmCollisionDefaultLocation);
	PickUpCollision->SetHiddenInGame(true);
	PickUpCollision->OnComponentBeginOverlap.AddDynamic(this, &AArm::OnPickUpCollisionOverlap);
	PickUpCollision->OnComponentEndOverlap.AddDynamic(this, &AArm::OnPickUpCollisionEndOverlap);
	
	SetRootComponent(PickUpCollision);

	//Creation of the Arm Mesh and attachment to the PickupCollision
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(*ArmNameInScene);
	MeshComp->SetHiddenInGame(false);
	MeshComp->SetRelativeLocation(ArmMeshDefaultLocation);
	MeshComp->SetRelativeRotation(ArmMeshDefaultRotation);
	MeshComp->SetupAttachment(PickUpCollision);

	auto dataTable = ConstructorHelpers::FObjectFinder<UDataTable>(*ArmDataTableFilePath);
	DataTable = dataTable.Object;

	 //Not functional that will be an arm randomizer
	//if (isConstructor)
	//{
	//	//Chossing Random Arm in the beginning
	//	switch (RandomArmNum)
	//	{
	//
	//
	//
	//	case 1:
	//		SetEditorArmStats(FString(TEXT("Necro")));
	//		
	//		break;
	//
	//
	//	case 2:
	//		SetEditorArmStats(FString(TEXT("Flamethrower")));
	//
	//		break;
	//
	//
	//	case 3:
	//		SetEditorArmStats(FString(TEXT("Bleed")));
	//
	//		break;
	//
	//
	//	case 4:
	//		SetEditorArmStats(FString(TEXT("Venom")));
	//
	//		break;
	//
	//
	//	case 5:
	//		SetEditorArmStats(FString(TEXT("Paralyze")));
	//
	//
	//		break;
	//
	//
	//	case 6:
	//
	//		SetEditorArmStats(FString(TEXT("Brute")));
	//
	//		break;
	//	default:
	//		break;
	//	}
	//	SetArmDataFromTable();
	//}
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsConstructor = false;
}

/// <summary>
/// Changes in the editor will be saved and the arm will change
/// </summary>
/// <param name="PropertyChangedEvent"></param>
#if WITH_EDITOR
void AArm::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetArmDataFromTable();
}
#endif

// Called when the game starts or when spawned
void AArm::BeginPlay()
{
	Super::BeginPlay();
	
	if (const auto hud = GetWorld()->GetFirstPlayerController()->GetHUD(); hud->IsA<ALevelHUD>())
		LevelHud = Cast<ALevelHUD>(hud);

	SetArmDataFromTable();
}

// Called every frame
void AArm::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	PickUpCollision->SetRelativeRotation(GetActorRotation() + ArmSpiningRotation);
}

void AArm::SetType(const EArmType& A_ArmType)
{
	ArmType = A_ArmType;
}

//Set Arm Stats From Data Table
void AArm::SetArmDataFromTable()
{
	auto tableRowName = FName(UEnum::GetDisplayValueAsText(ArmType).ToString());

	if (auto data = DataTable->FindRow<FArmData>(tableRowName, ""); data)
	{
		ArmData = *data;
	}

	if (ArmData.ArmWroldMesh) MeshComp->SetSkeletalMesh(ArmData.ArmWroldMesh.Get());
}

/// <summary>
/// Wen the player colides with the Arm collision zone
/// switching the arm to the type that is laying in the world
/// </summary>
/// <param name="OverlappedComponent"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
/// <param name="bFromSweep"></param>
/// <param name="SweepResult"></param>
void AArm::OnPickUpCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ASnapOutOfItCharacter::StaticClass())) return;

	auto character = Cast<ASnapOutOfItCharacter>(OtherActor);
	character->SetArmInRange(this);

	//Show Arm Ui
	LevelHud->ShowPickUpUI();

	//Looking witch type the arm has so the ui can show it right
	if (static_cast<uint8>(ArmData.ArmType) == static_cast<uint8>(ArmType))
	{
		ArmData.ArmType = static_cast<EArmType>(static_cast<uint8>(ArmType));

		switch (ArmType)
		{
		case EArmType::ET_DEFAULT:

			LevelHud->SetArmName(ArmNameDefault);

			break;

		case EArmType::ET_MELTED_ARM:
			LevelHud->SetArmName(ArmNameMelted);

			break;


		case EArmType::ET_NECRO_ARM:
			LevelHud->SetArmName(ArmNameNecro);

			break;


		case EArmType::ET_FLAMETHROWER:
			LevelHud->SetArmName(ArmNameFlamethrower);

			break;


		case EArmType::ET_BLEED_ARM:
			LevelHud->SetArmName(ArmNameBleed);

			break;


		case EArmType::ET_VENOM_ARM:
			LevelHud->SetArmName(ArmNameVenom);

			break;


		case EArmType::ET_PARALYZE:
			LevelHud->SetArmName(ArmNameParalyze);

			break;


		case EArmType::ET_BRUTE_ARM:
			LevelHud->SetArmName(ArmNameBrute);

			break;
		default:
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, ;
			break;
		}
	}
}

/// <summary>
/// when the player leaves the collision zone
/// -hide the UI
/// -SetArmNotInRange macht das mann den arm jetzt nicht mehr aufheben kann
/// </summary>
/// <param name="OverlappedComponent"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
void AArm::OnPickUpCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ASnapOutOfItCharacter::StaticClass()))
	{
		auto character = Cast<ASnapOutOfItCharacter>(OtherActor);
		character->SetArmNotInRange();								
	}

	LevelHud->HidePickUpUI();
}

void AArm::PickUp(AActor* Interactor)
{
	IInteractable::PickUp(Interactor);

	OnArmPickUpSignature.Broadcast(this->ArmData);
	
	Destroy();
}
