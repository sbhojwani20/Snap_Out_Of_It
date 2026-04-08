// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../SnapOutOfItCharacter.h"
#include "UObject/UnrealType.h"
#include "Emil/ArmData.h"
#include "Emil/UI/LevelHUD.h"
#include "Objects/Tobias/Interactable.h"
#include "Arm.generated.h"

class USphereComponent;
class UUPickUpUI;
struct FArmData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArmPickUpSignature, const FArmData&, ArmType);

UCLASS()
class AArm : public AActor, public IInteractable
{
	GENERATED_BODY()

#pragma region Consts
	const FString ArmDataTableFilePath = FString(
		TEXT("/Script/Engine.DataTable'/Game/DataTables/Emil/DT_ArmData.DT_ArmData'"));

	//Arm Names
	const FString ArmNameMelted = FString(TEXT("Melted"));
	const FString ArmNameBleed = FString(TEXT("Bleed"));
	const FString ArmNameParalyze = FString(TEXT("Paralyze"));
	const FString ArmNameFlamethrower = FString(TEXT("Flamethrower"));
	const FString ArmNameVenom = FString(TEXT("Venom"));
	const FString ArmNameNecro = FString(TEXT("Necro"));
	const FString ArmNameBrute = FString(TEXT("Brute"));
	const FString ArmNameDefault = FString(TEXT("Default"));
	const FString ArmColisionBoxName = FString(TEXT("Pick up Zone"));
	const FString ArmNameInScene = FString(TEXT("Arm"));

	//Locations
	const FVector ArmMeshDefaultLocation = FVector(-120, 0, -14);
	const FRotator ArmMeshDefaultRotation = FRotator(-90, 0, 0);
	
	const FVector ArmCollisionDefaultLocation = FVector(0, 0, 90);
	
	const FRotator ArmSpiningRotation = FRotator(0, 2, 0);
	const FRotator ArmRotator1 = FRotator(-90, 0, 0);
	const FRotator ArmRotator2 = FRotator(90, 0, 0);

	const float ArmCollisionRadiusDefault = 200.f;
#pragma endregion

public:
	// Sets default values for this actor's properties
	AArm();

#pragma region Methods
protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetType(const EArmType& A_ArmType);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FArmData GetArmData() const { return ArmData; }

	UFUNCTION(BlueprintCallable)
	virtual void PickUp(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable)
	void SetArmDataFromTable();
	
	UFUNCTION(BlueprintCallable)
	void OnPickUpCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnPickUpCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion

#pragma region Variables
	UPROPERTY(BlueprintAssignable, Category="Collision")
	FArmPickUpSignature OnArmPickUpSignature;
	
private:
	UPROPERTY(EditAnywhere, Category = "Arm Mesh")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Pick up Zone")
	USphereComponent* PickUpCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arm Data info",
		meta = (RequiredAssetDataTags = "RowStructure=/Script/SnapOutOfIt.ArmData", AllowPrivateAccess = "true"))
	UDataTable* DataTable = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pick up Zone")
	ALevelHUD* LevelHud = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Arm Info")
	EArmType ArmType{EArmType::ET_DEFAULT};
	
	UPROPERTY(EditAnywhere, Category = "Arm info")
	FArmData ArmData{};
	
	bool bIsConstructor = false;
#pragma endregion
};
