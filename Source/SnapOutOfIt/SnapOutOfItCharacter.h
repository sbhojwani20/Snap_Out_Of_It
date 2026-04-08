// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Emil/ArmData.h"
#include "Tobias/Combat/CombatEntity.h"
#include "Logging/LogMacros.h"
#include "Shayna/Character/CharacterData.h"
#include "SnapOutOfItCharacter.generated.h"

class ALevelHUD;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AArm;
class IInteractable;
enum class EEntityType : uint8;
struct FCharacterData;
struct FArmData;
struct FWorldData;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmSwapSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArmChargeChangeSignature, const uint8&, Charge);

UENUM(BlueprintType)
enum class ESelectionType : uint8
{
	ST_DEFAULT UMETA(DisplayName="Default"),
	ST_LEFT_ARM UMETA(DisplayName="LeftArm"),
	ST_RIGHT_ARM UMETA(DisplayName="RightArm"),
	ST_INVENTORY UMETA(DisplayName="Inventory"),
	ST_COUNT UMETA(DisplayName="Count")
};

UCLASS(config=Game)
class ASnapOutOfItCharacter : public ACombatEntity
{
	GENERATED_BODY()

#pragma region Character Movement Properties
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
#pragma endregion

public:
	ASnapOutOfItCharacter();

protected:
	// To add mapping context
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* a_playerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//---------------------------------------------------------------------------------------------
	// Modified class by us

#pragma region Consts
#pragma region Code from Emil
	//Arm Names
	const FString LEFT_ARM_COMPONENT_NAME = TEXT("Left Arm");
	const FString RIGHT_ARM_COMPONENT_NAME = TEXT("Right Arm");

	const FString LEFT_PICK_UP_INPUT_ACTION_PATH = TEXT(
		"/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Left_Pick_Up.IA_Left_Pick_Up'");
	const FString RIGHT_PICK_UP_INPUT_ACTION_PATH = TEXT(
		"/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Right_Pick_Up.IA_Right_Pick_Up'");
	const FString PAUSE_INPUT_ACTION_PATH = TEXT(
		"/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Pause.IA_Pause'");
#pragma endregion

#pragma	region Code from Shayna
	const FString WORLD_DATA_TABLE_PATH = TEXT("/Script/Engine.DataTable'/Game/DataTables/Shayna/DT_World.DT_World'");
	const FString CHARACTER_DATA_TABLE_PATH = TEXT(
		"/Script/Engine.DataTable'/Game/DataTables/Shayna/DT_Character.DT_Character'");
	const FString ARM_DATA_TABLE_PATH = TEXT("/Script/Engine.DataTable'/Game/DataTables/Emil/DT_ArmData.DT_ArmData'");

	const FVector MESH_GROUND_OFFSET = FVector(0, 0, -95);
	const FRotator MESH_FACE_DIR_OFFSET = FRotator(0, -90, 0);
#pragma endregion

#pragma region Code from Tobias
	const uint8 MAX_ARM_CHARGES = 5;
#pragma endregion
#pragma endregion

#pragma region Methods

private:
	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#pragma region Code from Emil

public:
	UFUNCTION(BlueprintCallable)
	inline void SetArmInRange(AArm* newArm) { ArmInRange = newArm; }

	UFUNCTION(BlueprintCallable)
	inline void SetArmNotInRange() { ArmInRange = nullptr; } //ToDo wenn spieler nicht in range

	USkeletalMeshComponent* InitialiseArm(const FString& armComponentName);

	void PickUpLeft();
	void PickUpRight();
	void PauseMenu();

	UFUNCTION()
	void SaveArmInInventory(const FArmData& armData);
#pragma endregion

#pragma region Code from Tobias
	UFUNCTION()
	FORCEINLINE FCharacterData& GetCharacterData() { return CharacterData; }

	UFUNCTION()
	FORCEINLINE ESelectionType& GetAttackSelectionType() { return AttackSelectionType; }

	UFUNCTION()
	FORCEINLINE uint8& GetArmCharges() { return ArmCharges; }

	UFUNCTION()
	void PickUpArm(const FArmData& armData);

	UFUNCTION()
	bool HasEnoughCharges();

	UFUNCTION()
	void SetAttackSelectionType(const ESelectionType& selectionType);

	UFUNCTION()
	void SetArmData(const FArmData& armData);

	UFUNCTION()
	void SetArmCharges(const uint8& armCharges);

	UFUNCTION()
	void SetArmDataFromTable(const EArmType armType, const ESelectionType& armSideSelection);

	UFUNCTION()
	void HandleArmSwitchClicked(const uint8& inventoryIdx, const ESelectionType& armSideSelection);

	UFUNCTION()
	void FinishArmSwitch();

	UFUNCTION()
	void SwapArms();

	UFUNCTION()
	FORCEINLINE void SetAttackType(const bool& a_bType) { bSpecialAttack = a_bType; }

	UFUNCTION()
	void UseHealFlask();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimInstance(USkeletalMeshComponent* armMesh, UAnimInstance* animInstance);

	virtual void AttackTarget() override;

	UFUNCTION()
	void SpecialAttackTarget();

	virtual void CheckStatusEffect() override;

	virtual void ReceiveDamage(const int32& a_damage) override;

	virtual void HandleDeath() override;

	virtual bool CheckDeathConditions() override;
	UFUNCTION()
	void LoadCollectedArm();

	UFUNCTION()
	void LoadActiveArms();

	UFUNCTION()
	void ClearCollectedArms();

	UFUNCTION()
	void ApplySpecialDamageToTarget();

	UFUNCTION()
	void PlayLeftArmAnimation(UAnimSequence* animationToPlay, const bool& bAnimLoop) const;

	UFUNCTION()
	void PlayRightArmAnimation(UAnimSequence* animationToPlay, const bool& bAnimLoop) const;

#pragma endregion

#pragma region Shaynas Code

public:
	UFUNCTION()
	void SaveLastPosition();

	UFUNCTION()
	void SetPlayerWorldPosition();

	UFUNCTION()
	FORCEINLINE void SetLastPlayerLocation(const FVector& a_fCurrentLocation)
	{
		LastPlayerLocation = a_fCurrentLocation;
	}

	UFUNCTION()
	FORCEINLINE void SetLastPlayerRotation(const FRotator& a_fCurrentRotation)
	{
		LastPlayerRotation = a_fCurrentRotation;
	}

	UFUNCTION()
	FORCEINLINE void SetLastLevelName(const FName& a_fCurrentLevel) { LastLevelName = a_fCurrentLevel; }

	UFUNCTION()
	FORCEINLINE UDataTable* GetWorldDataTable() const { return WorldDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Grappling Hook")
	void SetGrappleActive(bool bGrappleActive);
#pragma endregion
#pragma endregion

#pragma region Variables

protected:
#pragma region Code from Emil
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArmData, meta = (AllowPrivateAccess = "true"))
	FArmData LeftArmData{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ArmData, meta = (AllowPrivateAccess = "true"))

	FArmData RightArmData{};

	UPROPERTY(EditAnywhere)
	AArm* ArmInRange = nullptr;

	UPROPERTY(EditAnywhere)
	ALevelHUD* LevelHud = nullptr;

	//todo allow private accse + UPROPERTY(EditAnywhere, Category = Test, BlueprintReadWrite, meta = (MakeEditWidget = ��))

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Arm, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LeftArmMeshComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Arm, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RightArmMeshComp = nullptr;

	//Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickUpArmLeft = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickUpArmRight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Pause = nullptr;

#pragma endregion

#pragma region Code from Tobias
	
public:
	UPROPERTY(BlueprintAssignable)
	FArmSwapSignature OnArmSwap;

	UPROPERTY(BlueprintAssignable)
	FArmChargeChangeSignature OnArmChargeChange;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereColl = nullptr;

	UPROPERTY(EditAnywhere)
	ESelectionType EquipSelectionType{ESelectionType::ST_DEFAULT};

	UPROPERTY(EditAnywhere)
	ESelectionType AttackSelectionType{ESelectionType::ST_DEFAULT};

	UPROPERTY(EditAnywhere)
	uint8 InventorySwapIdx = 0;

	UPROPERTY(EditAnywhere)
	uint8 ArmCharges = 0;

	UPROPERTY(EditAnywhere)
	bool bSpecialAttack = false;
#pragma endregion

#pragma region Shaynas Code
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	FVector LastPlayerLocation;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	FRotator LastPlayerRotation;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	FName LastLevelName;

	UPROPERTY()
	UDataTable* WorldDataTable = nullptr;

	UPROPERTY()
	UDataTable* CharacterDataTable = nullptr;

	UPROPERTY()
	UDataTable* ArmDataTable = nullptr;

	UPROPERTY()
	FWorldData WorldData{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Info")
	FCharacterData CharacterData{};
#pragma endregion
#pragma endregion
};
