// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tobias/Combat/CombatData.h"
#include "Shayna/World/WorldData.h"
#include "GameFramework/Character.h"
#include "DamageTypes.h"
#include "CombatEntity.generated.h"

class UCombatManager;
UCLASS()
class ACombatEntity : public ACharacter
{
	GENERATED_BODY()

private:
	const FString ENTITY_DATA_TABLE_PATH = TEXT("/Script/Engine.DataTable'/Game/DataTables/Tobias/DT_Entity.DT_Entity'");

	const int32 STATUS_EFFECT_ROUNDS = 5;
	const int32 PARALYSIS_EFFECT_ROUNDS = 2;
	const int32 NO_STATUS_EFFECT_ROUNDS = 0;
	const int32 FIRE_DAMAGE = 10;

	const FText STATUS_EFFECT_NAME_NONE = FText();
	const FText STATUS_EFFECT_NAME_PARALYSIS = FText::FromString(TEXT("Paralyzed"));
	const FText STATUS_EFFECT_NAME_FIRE = FText::FromString(TEXT("Burning"));
	const FText STATUS_EFFECT_NAME_POISON = FText::FromString(TEXT("Poisoned"));
	const FText STATUS_EFFECT_NAME_BLEED = FText::FromString(TEXT("Bleeding"));

public:
	// Sets default values for this actor's properties
	ACombatEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR

public:
	UFUNCTION()
	FORCEINLINE EEntityType& GetType() { return Type; };
	
	UFUNCTION()
	FORCEINLINE FCombatData& GetStats() { return EntityData; };
	
	UFUNCTION()
	FORCEINLINE UCombatManager* GetManager() const { return Manager; };
	
	UFUNCTION()
	FORCEINLINE ACombatEntity* GetTarget() const { return Target; };

	UFUNCTION()
	virtual FORCEINLINE void SetTarget(ACombatEntity* a_target) { Target = a_target; }

	UFUNCTION()
	virtual FORCEINLINE void SetType(const EEntityType& a_type) {	Type = a_type; }

	UFUNCTION()
	virtual FORCEINLINE void SetDamageModifier(const float& a_modifier) { DamageModifier = a_modifier; }
	
	UFUNCTION()
	virtual void SetUpEntity();

	UFUNCTION()
	virtual void AttackTarget();
	
	UFUNCTION()
	void EndTurn();

	UFUNCTION()
	void ApplyDamageToTarget();
	
	UFUNCTION()
	void ApplyStatusEffect(const E_DAMAGE_TYPE& a_damageType);

	UFUNCTION()
	void SetStatusEffect(const EStatusEffectType& a_statusEffect);

	UFUNCTION()
	virtual void CheckStatusEffect();

	UFUNCTION()
	virtual bool CheckParalyseEffect();
	
	UFUNCTION()
	virtual void HealDamage(const EHealType& a_healType);

	UFUNCTION()
	virtual void ReceiveDamage(const int32& a_damage);

	UFUNCTION()
	virtual bool CheckDeathConditions();

#pragma region Shaynas Code
	UFUNCTION()
	virtual void HandleDeath();
#pragma endregion

protected:
	UFUNCTION()
	virtual void SetDataFromRow();
	
	UFUNCTION()
	virtual int32 CalcDamageOutput();

	UFUNCTION()
	virtual int32 CalcSpecialDamageOutput(const int32& a_specialDamage);

private:
	UPROPERTY()
	UCombatManager* Manager = nullptr;

	UPROPERTY(EditAnywhere, Category = "Entity Info", meta = (RequiredAssetDataTags = "RowStructure=/Script/SnapOutOfIt.CombatData"))
	UDataTable* EntityDataTable = nullptr;

	UPROPERTY(EditAnywhere, Category = "Entity Info")
	EEntityType Type = EEntityType::ET_DEFAULT;

	UPROPERTY(EditAnywhere, Category = "Entity Info")
	FCombatData EntityData {};

	UPROPERTY(EditAnywhere)
	ACombatEntity* Target = nullptr;

	// UPROPERTY()
	// AActor* ButcherKnife = nullptr;
	
	UPROPERTY()
	FTimerHandle HandleEndOfTurn{};

	UPROPERTY(EditAnywhere, Category = "Entity Info")
	int32 StatusRounds = 0;
	
	UPROPERTY(EditAnywhere, Category = "Entity Info")
	int32 ParalyseRounds = 0;
	
	UPROPERTY(EditAnywhere, Category = "Entity Info")
	int32 DamageTaken = 0;

	UPROPERTY(EditAnywhere, Category = "Entity Info")
	float DamageModifier = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 TimerCount = 0;

	UPROPERTY()
	bool bIsMyTurn = false;
	
	UPROPERTY()
	bool bEffectedThisTurn = false;
};
