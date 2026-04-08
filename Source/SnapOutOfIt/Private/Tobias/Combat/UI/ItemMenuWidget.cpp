// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/UI/ItemMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Tobias/Combat/CombatManager.h"


void UItemMenuWidget::NativeConstruct()
{
	if (HealFlaskButton)
	{
		HealFlaskButton->OnClicked.AddDynamic(this, &UItemMenuWidget::HealFlaskButtonClicked);
	}

	Manager = UCombatManager::GetManagerInstance(*this);

	SetVisibility(ESlateVisibility::Hidden);
}

void UItemMenuWidget::HealFlaskButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);

	Manager->HandleHealFlaskClicked();
}

void UItemMenuWidget::SetHealFlaskCount(const uint32& a_count) const
{
	HealFlaskCount->SetText(FText::FromString(FString::FromInt(a_count)));
}
