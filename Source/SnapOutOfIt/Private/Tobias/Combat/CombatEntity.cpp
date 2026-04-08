// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/CombatEntity.h"

#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Shayna/Enemy/EnemyAI.h"
#include "SnapOutOfIt/SnapOutOfItCharacter.h"
#include "Tobias/Combat/CombatData.h"
#include "Tobias/Combat/CombatManager.h"

ACombatEntity::ACombatEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto entityDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(*ENTITY_DATA_TABLE_PATH);

	if (entityDataTable.Succeeded()) EntityDataTable = entityDataTable.Object;
	if (!EntityDataTable) return;
	// if (GEngine) GEngine->AddOnScreenDebugMessage(20, 10.0f, FColor::Blue, EntityDataTable->GetName());

	DamageModifier = 1.0f;
}

// Called when the game starts or when spawned
void ACombatEntity::BeginPlay()
{
	Super::BeginPlay();

	Manager = UCombatManager::GetManagerInstance(*this);
}

// Called every frame
void ACombatEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (StatusRounds > 0) if (GEngine) GEngine->AddOnScreenDebugMessage(54, 5.0f, FColor::Blue, FString::Printf(TEXT("Status Rounds: %i"), StatusRounds));
}

#if WITH_EDITOR
void ACombatEntity::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetUpEntity();
}
#endif WITH_EDITOR

void ACombatEntity::SetUpEntity()
{
	if (!EntityDataTable)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-10, 10.0f, FColor::Red, TEXT("DataTable not found!"));

		return;
	}
	SetDataFromRow();
}

void ACombatEntity::AttackTarget()
{
	bIsMyTurn = true;
	bEffectedThisTurn = false;
}

void ACombatEntity::EndTurn()
{
	if (this->IsA<ASnapOutOfItCharacter>()) Cast<ASnapOutOfItCharacter>(this)->CheckStatusEffect();
	else CheckStatusEffect();

	GetWorldTimerManager().SetTimer(HandleEndOfTurn, Manager, &UCombatManager::EndTurn, 2.f, false);

	bIsMyTurn = false;
}

void ACombatEntity::ApplyDamageToTarget()
{
	Target->ReceiveDamage(CalcDamageOutput());
}

void ACombatEntity::ApplyStatusEffect(const E_DAMAGE_TYPE& a_damageType)
{
	switch (a_damageType)
	{
	case E_DAMAGE_TYPE::DT_DEFAULT:
		SetStatusEffect(EStatusEffectType::SE_DEFAULT);
		SetDamageModifier(1.0f);
		break;
	case E_DAMAGE_TYPE::DT_FIRE:
		SetStatusEffect(EStatusEffectType::SE_BURNED);
		SetDamageModifier(1.0f);

		break;
	case E_DAMAGE_TYPE::DT_POISON:
		SetStatusEffect(EStatusEffectType::SE_POISONED);
		SetDamageModifier(0.8f);

		break;
	case E_DAMAGE_TYPE::DT_PARALYSIS:
		SetStatusEffect(EStatusEffectType::SE_PARALIZED);
		SetDamageModifier(1.0f);

		break;
	case E_DAMAGE_TYPE::DT_BLEED:
		SetStatusEffect(EStatusEffectType::SE_BLEEDING);
		SetDamageModifier(1.0f);

		break;
	default:
		break;
	}
}

void ACombatEntity::SetStatusEffect(const EStatusEffectType& a_statusEffect)
{
	EntityData.StatusEffect = a_statusEffect;
	if (a_statusEffect == EStatusEffectType::SE_PARALIZED) ParalyseRounds = PARALYSIS_EFFECT_ROUNDS;
	else StatusRounds = STATUS_EFFECT_ROUNDS;

	switch (a_statusEffect)
	{
	case EStatusEffectType::SE_BURNED:
		Manager->SetStatusEffectInUI(STATUS_EFFECT_NAME_FIRE);
		break;
	case EStatusEffectType::SE_BLEEDING:
		Manager->SetStatusEffectInUI(STATUS_EFFECT_NAME_BLEED);
		break;
	case EStatusEffectType::SE_POISONED:
		Manager->SetStatusEffectInUI(STATUS_EFFECT_NAME_POISON);
		break;
	case EStatusEffectType::SE_PARALIZED:
		Manager->SetStatusEffectInUI(STATUS_EFFECT_NAME_PARALYSIS);
		break;
	default:
		Manager->SetStatusEffectInUI(STATUS_EFFECT_NAME_NONE);
		break;
	}
}

void ACombatEntity::CheckStatusEffect()
{
	if (EntityData.StatusEffect == EStatusEffectType::SE_DEFAULT)
	{
		GetMesh()->PlayAnimation(EntityData.IdleCombatAnimSequ.Get(), true);
		return;
	}
	if (StatusRounds <= NO_STATUS_EFFECT_ROUNDS && ParalyseRounds <= NO_STATUS_EFFECT_ROUNDS)
	{
		ApplyStatusEffect(E_DAMAGE_TYPE::DT_DEFAULT);

		GetMesh()->PlayAnimation(EntityData.IdleCombatAnimSequ.Get(), true);
		return;
	}
	if (bIsMyTurn && !bEffectedThisTurn)
	{
		switch (GetStats().StatusEffect)
		{
		case EStatusEffectType::SE_BURNED:
			ReceiveDamage(FIRE_DAMAGE);
			break;
		case EStatusEffectType::SE_BLEEDING:
			ReceiveDamage(FIRE_DAMAGE * (StatusRounds / 3));
			break;
		case EStatusEffectType::SE_POISONED:
			GetMesh()->PlayAnimation(EntityData.EffectedAnimSequ.Get(), true);
			break;
		case EStatusEffectType::SE_PARALIZED:
			GetMesh()->PlayAnimation(EntityData.EffectedAnimSequ.Get(), true);
			break;
		default:
			break;
		}
		StatusRounds--;

		bEffectedThisTurn = true;
		return;
	}

	GetMesh()->PlayAnimation(EntityData.EffectedAnimSequ.Get(), true);
}

bool ACombatEntity::CheckParalyseEffect()
{
	if (EntityData.StatusEffect == EStatusEffectType::SE_PARALIZED)
	{
		ParalyseRounds--;

		EndTurn();

		return true;
	}
	return false;
}

void ACombatEntity::HealDamage(const EHealType& a_healType)
{
	switch (a_healType)
	{
	case EHealType::HT_DEFAULT:
		break;
	case EHealType::HT_HEAL_FLASK:
		if (EntityData.Health += 50; EntityData.Health > EntityData.MaxHealth) EntityData.Health = EntityData.MaxHealth;

		Manager->SetHealthBar(static_cast<float>(EntityData.Health) / static_cast<float>(EntityData.MaxHealth), this);
		break;
	case EHealType::HT_LIFE_STEAL:
		if (EntityData.Health += CalcDamageOutput(); EntityData.Health > EntityData.MaxHealth) EntityData.Health = EntityData.MaxHealth;

		Manager->SetHealthBar(static_cast<float>(EntityData.Health) / static_cast<float>(EntityData.MaxHealth), this);
	default: ;
	}
	
}

bool ACombatEntity::CheckDeathConditions()
{
	if (EntityData.Health == 0)
	{
		if (EntityData.DeathAnimSequ)
		{
			GetMesh()->PlayAnimation(EntityData.DeathAnimSequ.Get(), false);
		}
		else HandleDeath();

		return true;
	}

	return false;
}

#pragma region Shaynas Code
void ACombatEntity::HandleDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan,
	                                 FString::Printf(TEXT("HandleDeath in CombatEntity aufgerufen.")));

	if (this->IsA<AEnemyAI>() && Manager) Manager->OnEnemyDefeated();
	if (this->IsA<ASnapOutOfItCharacter>())
	{
		GEngine->AddOnScreenDebugMessage(-14, 8, FColor::Red, TEXT("Player is defeated"));
		Manager->OnPlayerDefeated();
	}
}
#pragma endregion

void ACombatEntity::SetDataFromRow()
{
	auto tableRowName = FName(UEnum::GetDisplayValueAsText(Type).ToString());

	if (auto data = EntityDataTable->FindRow<FCombatData>(tableRowName, ""); data)
	{
		EntityData = *data;
	}
	if (EntityData.EntityMesh) GetMesh()->SetSkeletalMesh(EntityData.EntityMesh.Get());
}

int32 ACombatEntity::CalcDamageOutput()
{
	if (EntityData.Attack * DamageModifier <= Target->EntityData.Defense) return 1;

	return (EntityData.Attack * DamageModifier) - Target->EntityData.Defense;
}

int32 ACombatEntity::CalcSpecialDamageOutput(const int32& a_specialDamage)
{
	if ((EntityData.Attack + a_specialDamage) * DamageModifier <= Target->EntityData.Defense) return 1;

	return (EntityData.Attack + a_specialDamage) * DamageModifier - Target->EntityData.Defense;
}

void ACombatEntity::ReceiveDamage(const int32& a_damage)
{
	if (EntityData.Health -= a_damage; EntityData.Health < 0) EntityData.Health = 0;

	Manager->SetHealthBar(static_cast<float>(EntityData.Health) / static_cast<float>(EntityData.MaxHealth), this);

	if (EntityData.HitAnimSequ) GetMesh()->PlayAnimation(EntityData.HitAnimSequ.Get(), false);
}
