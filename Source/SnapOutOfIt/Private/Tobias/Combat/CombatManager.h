 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shayna/Enemy/EnemyData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tobias/Combat/CombatData.h"
#include "CombatManager.generated.h"

class UCombatUI;
class AEnemySpawn;
class ACombatEntity;
class UHitAnimNotify;

/**
 *
 */

UCLASS()
class UCombatManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static auto GetManagerInstance(const UObject& a_target) -> UCombatManager* const;

private:
	static auto GetGameInstance(const UObject& a_target) -> UGameInstance* const;

public:
	virtual void Initialize(FSubsystemCollectionBase& a_collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION()
	FORCEINLINE ACombatEntity* GetPlayer() const { return Player; }

	UFUNCTION()
	FORCEINLINE ACombatEntity* GetEnemy() const { return Enemy; }

	UFUNCTION()
	FORCEINLINE EEntityType GetEncounter() const { return Encounter; }

	UFUNCTION()
	FORCEINLINE void SetEncounter(const EEntityType& a_type) { Encounter = a_type; }
	
	UFUNCTION()
	void SetEnemyName(const FString& a_name);

	UFUNCTION()
	FORCEINLINE void SetEnemyDataTable(UDataTable* a_dataTable) { EnemyDataTable = a_dataTable; }

	UFUNCTION()
	FORCEINLINE void SetEnemyAliveBool(const bool& a_alive) { EnemyData.bEnemyAlive = a_alive; }

	UFUNCTION()
	FORCEINLINE void SetUI(UCombatUI* a_uI) { CombatUI = a_uI; }

	UFUNCTION()
	FORCEINLINE void SetPlayer(ACombatEntity* a_player) { Player = a_player; }

	UFUNCTION()
	FORCEINLINE void SetEnemy(ACombatEntity* a_enemy) { Enemy = a_enemy; }

	UFUNCTION()
	FORCEINLINE void SetEnemyStart(AEnemySpawn* a_spawn) { EnemyStart = a_spawn; }
	
	UFUNCTION()
	FORCEINLINE void SetPlayerStart(AActor* a_playerStart) { PlayerStart = a_playerStart; }

	UFUNCTION()
	void SetHealthBar(const float& a_health , ACombatEntity* a_target) const;

	UFUNCTION()
	void SetStatusEffectInUI(const FText& effectName) const;

	UFUNCTION()
	void SetArmInUI() const;

	UFUNCTION()
	void SetArmIconInUI(UTexture2D* icon, const ESelectionType& armSide) const;

	UFUNCTION()
	void StartCombat();

	UFUNCTION()
	void StartNextRound();

	UFUNCTION()
	void StartNextTurn();

	UFUNCTION()
	void EndTurn();

	UFUNCTION()
	void HandleAttackClicked() const;

	UFUNCTION()
	void HandleSpecialAttackClicked() const;

	UFUNCTION()
	void HandleHealFlaskClicked();

#pragma region Shaynas Code

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	FName LastLevelName;

	UFUNCTION()
	void OnEnemyDefeated();

	UFUNCTION()
	void OnPlayerDefeated() const;

	void SetLastLevel(const FName &a_lastLevelName) { LastLevelName = a_lastLevelName; }

#pragma endregion

private:
	TQueue<ACombatEntity*, EQueueMode::Spsc> AttackOrder{};

	UPROPERTY(VisibleAnywhere)
	ACombatEntity* ActiveEntity = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCombatUI* CombatUI = nullptr;

	UPROPERTY(VisibleAnywhere)
	ACombatEntity* Player = nullptr;

	UPROPERTY(VisibleAnywhere)
	ACombatEntity* Enemy = nullptr;

	UPROPERTY(VisibleAnywhere)
	AEnemySpawn* EnemyStart = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	AActor* PlayerStart = nullptr;

	UPROPERTY(VisibleAnywhere)
	EEntityType Encounter = EEntityType::ET_DEFAULT;

	UPROPERTY(VisibleAnywhere)
	FEnemyData EnemyData{};
	
	UPROPERTY(VisibleAnywhere)
	UDataTable* EnemyDataTable = nullptr;

	UPROPERTY(EditAnywhere, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 HealFlaskCharges = 0;
};
