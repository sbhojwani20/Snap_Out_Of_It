// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/CombatManager.h"
#include "Tobias/Combat/UI/CombatUI.h"
#include "Shayna/Enemy/EnemyAI.h"
#include "../SnapOutOfItCharacter.h"
#include "Emil/UI/LevelHUD.h"
#include "Kismet/GameplayStatics.h"

// Code from Marcus
//-----------------------------------------------
auto UCombatManager::GetManagerInstance(const UObject& a_target) -> UCombatManager* const
{
	auto gameInstance = GetGameInstance(a_target);

	if (gameInstance) return gameInstance->GetSubsystem<UCombatManager>();

	return nullptr;
}

auto UCombatManager::GetGameInstance(const UObject& a_target) -> UGameInstance* const
{
	if (a_target.GetWorld() && a_target.GetWorld()->GetGameInstance())
	{
		return UGameplayStatics::GetGameInstance(a_target.GetWorld());
	}
	else if (a_target.IsA(AActor::StaticClass()))
	{
		if (Cast<AActor>(&a_target)->GetGameInstance()) return GEngine->GetWorld()->GetGameInstance();
	}

	return nullptr;
}

//-----------------------------------------------

void UCombatManager::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Display, TEXT("Combat Manager initialized!"));
}

void UCombatManager::Deinitialize()
{
	UE_LOG(LogTemp, Display, TEXT("Combat Manager finalized!"));
}

void UCombatManager::SetEnemyName(const FString& a_name)
{
	EnemyData.EnemyName = a_name;
}

void UCombatManager::SetHealthBar(const float& a_health, ACombatEntity* a_target) const
{
	if (a_target->IsA<ASnapOutOfItCharacter>()) CombatUI->SetPlayerHealthBarPercent(a_health);
	if (a_target->IsA<AEnemyAI>()) CombatUI->SetEnemyHealthBarPercent(a_health);
}

void UCombatManager::SetStatusEffectInUI(const FText& effectName) const
{
	if (CombatUI) CombatUI->SetStatusEffect(effectName);
}

void UCombatManager::SetArmInUI() const
{
	if (CombatUI) CombatUI->SetArmName();
}

void UCombatManager::SetArmIconInUI(UTexture2D* icon, const ESelectionType& armSide) const
{
	if (CombatUI) CombatUI->SetArmIcon(icon, armSide);
}

void UCombatManager::StartCombat()
{
	if (CombatUI) CombatUI->InitializeUI();

	AttackOrder.Empty();

	HealFlaskCharges = 2;

	if (auto character = Cast<ASnapOutOfItCharacter>(Player); character) character->SetArmCharges(2);

	if (CombatUI) CombatUI->SetHealFlaskCount(HealFlaskCharges);
	if (CombatUI) CombatUI->SetEnemyName(Enemy->GetStats().Name);
	if (CombatUI) CombatUI->SetStatusEffect(FText());

	StartNextRound();
}

void UCombatManager::StartNextRound()
{
	if (!Player || !Enemy) return;

	Cast<ASnapOutOfItCharacter>(Player)->SetAttackSelectionType(ESelectionType::ST_RIGHT_ARM);

	Player->SetTarget(Enemy);
	Enemy->SetTarget(Player);

	AttackOrder.Enqueue(Player);
	AttackOrder.Enqueue(Player);
	AttackOrder.Enqueue(Enemy);

	StartNextTurn();
}

void UCombatManager::StartNextTurn()
{
	if (AttackOrder.Dequeue(ActiveEntity) && Player && Enemy)
	{
		if (!ActiveEntity) return;
		if (ActiveEntity->IsA<ASnapOutOfItCharacter>())
		{
			auto character = Cast<ASnapOutOfItCharacter>(ActiveEntity);

			switch (character->GetAttackSelectionType())
			{
			case ESelectionType::ST_LEFT_ARM:
				if (character->GetCharacterData().ActiveArmLeft == EArmType::ET_DEFAULT) EndTurn();
				break;
			case ESelectionType::ST_RIGHT_ARM:
				if (character->GetCharacterData().ActiveArmRight == EArmType::ET_DEFAULT) EndTurn();
				break;
			default: ;
			}
			CombatUI->SetUserSelectionVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CombatUI->SetUserSelectionVisibility(ESlateVisibility::Hidden);

			Cast<AEnemyAI>(ActiveEntity)->AttackTarget();
		}
	}
	else StartNextRound();
}

void UCombatManager::EndTurn()
{
	StartNextTurn();
}

void UCombatManager::HandleAttackClicked() const
{
	CombatUI->SetUserSelectionVisibility(ESlateVisibility::Hidden);

	if (ActiveEntity && ActiveEntity->IsA<ASnapOutOfItCharacter>())
		Cast<ASnapOutOfItCharacter>(ActiveEntity)->AttackTarget();
}

void UCombatManager::HandleSpecialAttackClicked() const
{
	if (!ActiveEntity || !ActiveEntity->IsA<ASnapOutOfItCharacter>()) return;
	
	auto player = Cast<ASnapOutOfItCharacter>(ActiveEntity);
	
	if (!player->HasEnoughCharges()) return;
	
	CombatUI->SetUserSelectionVisibility(ESlateVisibility::Hidden);
	player->SpecialAttackTarget();
}

void UCombatManager::HandleHealFlaskClicked()
{
	if (HealFlaskCharges > 0)
	{
		CombatUI->SetUserSelectionVisibility(ESlateVisibility::Hidden);

		Cast<ASnapOutOfItCharacter>(Player)->UseHealFlask();

		HealFlaskCharges--;

		CombatUI->SetHealFlaskCount(HealFlaskCharges);
	}
}

// go back to players last location
void UCombatManager::OnEnemyDefeated()
{
	EnemyData.bEnemyAlive = false;

	EnemyDataTable->AddRow(FName(EnemyData.EnemyName), EnemyData);

	GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("Kampf beendet. Wechsel zur�ck in Level: %s"),
		                                 *LastLevelName.ToString()));

	UGameplayStatics::OpenLevel(Player->GetWorld(), LastLevelName);
}

void UCombatManager::OnPlayerDefeated() const
{
	auto player = Cast<ASnapOutOfItCharacter>(Player);
	if (!EnemyDataTable) return;

	auto names = EnemyDataTable->GetRowNames();

	for (auto name : names)
	{
		EnemyDataTable->RemoveRow(name);
	}
	player->SetGrappleActive(false);
	player->ClearCollectedArms();

	if (auto hud = GetWorld()->GetFirstPlayerController()->GetHUD(); hud->IsA<ALevelHUD>())
		Cast<ALevelHUD>(hud)->ShowDeathScreen();
}

// todo

// save last location
// callback if enemy dies goes to worldmanager and worldmanager destroys the enemy
// worldmanager saves the last location and tells the combatmanager to spawn at the last location

// sfx
// actor class for audio with triggerbox
// beginplay has sound on false
// ontriggerentrer sound is true
// ontriggerexit sound is false
