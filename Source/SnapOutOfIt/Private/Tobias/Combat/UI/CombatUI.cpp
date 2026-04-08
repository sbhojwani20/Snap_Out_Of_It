// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/UI/CombatUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "SnapOutOfIt/SnapOutOfItCharacter.h"
#include "Tobias/Combat/UI/UserSelectionWidget.h"
#include "Tobias/Combat/CombatManager.h"

void UCombatUI::NativeConstruct()
{
	Manager = UCombatManager::GetManagerInstance(*this);

	if (!Manager) return;
	Manager->SetUI(this);
}

void UCombatUI::SetUserSelectionVisibility (const ESlateVisibility& a_access) const
{
	UserSelectionUI->SetVisibility(a_access);
}

void UCombatUI::SetArmChargeBar(const uint8& charge)
{
	auto barPercent = static_cast<float>(charge) / 5.f;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Charge: %f"), barPercent));
	
	ArmChargeBar->SetPercent(barPercent);
}

void UCombatUI::SetPlayerHealthBarPercent(const float& a_health) const
{
	HealthBarPlayer->SetPercent(a_health);
}

void UCombatUI::SetEnemyHealthBarPercent(const float& a_health) const
{
	HealthBarEnemy->SetPercent(a_health);
}

void UCombatUI::SetHealFlaskCount(const uint32& a_count) const
{
	UserSelectionUI->SetHealFlaskCount(a_count);
}

void UCombatUI::SetEnemyName(const FText& name) const
{
	EnemyName->SetText(name);
}

void UCombatUI::SetStatusEffect(const FText& effectName) const
{
	StatusEffect->SetText(effectName);
}

void UCombatUI::SetArmName() const
{
	UserSelectionUI->SetArmName();
}

void UCombatUI::SetArmIcon(UTexture2D* icon, const ESelectionType& armSide) const
{
	UserSelectionUI->SetActiveArmIcon(icon, armSide);
}

void UCombatUI::InitializeUI()
{
	if (auto player = Cast<ASnapOutOfItCharacter>(Manager->GetPlayer()); player)
		player->OnArmChargeChange.AddDynamic(this, &UCombatUI::SetArmChargeBar);
}
