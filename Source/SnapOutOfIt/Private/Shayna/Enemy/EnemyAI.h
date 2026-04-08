// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Tobias/Combat/CombatEntity.h"
#include "Shayna/Enemy/EnemyData.h"
#include "../SnapOutOfItCharacter.h"
#include "EnemyAI.generated.h"

class AWaypoint;
class UAudioComponent;
class ASFXTrigger;
UCLASS()
class AEnemyAI : public ACombatEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	FORCEINLINE bool GetCollidedWithPlayer() { return CollidedWithPlayer; }
	
	UFUNCTION()
	FORCEINLINE void SetCollidedWithPlayer(bool a_value) { CollidedWithPlayer = a_value; }
	
	virtual void AttackTarget() override;

	UPROPERTY(EditAnywhere, Category = "Enemy AI")
	TArray<AWaypoint*> Waypoints = {};

private:
	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CheckIfEnemyIsAlive();

	const FVector MESH_GROUND_OFFSET = FVector(0, 0, -90);
	
	const FRotator MESH_FACE_DIR_OFFSET = FRotator(0, -90, 0);

	const FString AI_MANAGER_PATH = TEXT("/Script/CoreUObject.Class'/Script/SnapOutOfIt.EnemyAIManager'");

	UPROPERTY(EditAnywhere, Category = "Enemy AI")
	USphereComponent* SphereColl = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent  = nullptr;
	
	UPROPERTY()
	bool CollidedWithPlayer = false;

	UPROPERTY()
	UDataTable* EnemyDataTable = nullptr;

	const FString ENEMY_DATA_TABLE_PATH = TEXT("/Script/Engine.DataTable'/Game/DataTables/Shayna/DT_Enemy.DT_Enemy'");

	UPROPERTY()
	FEnemyData EnemyData{};

	UPROPERTY(EditAnywhere)
	FString EnemyName = FString(TEXT(""));

#pragma region Code from Tobias
public:
	UFUNCTION()
	FORCEINLINE void SetEnemyName(const FString& a_name) { Rename(*a_name, nullptr); }

	UFUNCTION()
	void GiveEnemyDataToManager() const;
	
	virtual void SetUpEntity() override;
	
	virtual void HandleDeath() override;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR

private:
	const FString BUTCHER_KNIFE = TEXT("/Script/Engine.StaticMesh'/Game/Michael_Pretorius_Level/Meshs/Bosses/Butcher/Knife2_LP.Knife2_LP'");
	
	UPROPERTY()
	UStaticMesh* ButcherKnife  = nullptr;

	UPROPERTY()
	ASnapOutOfItCharacter* Player = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ArmSpawn1 = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ArmSpawn2 = nullptr;

	UPROPERTY()
	FVector SpawnPos = FVector::ZeroVector;
#pragma endregion 
};
