// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/UI/UserSelectionWidget.h"

#include "ArmSelectionWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../SnapOutOfItCharacter.h"
#include "Tobias/Combat/CombatManager.h"
#include "Tobias/Combat/UI/ItemMenuWidget.h"

void UUserSelectionWidget::NativeConstruct()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	if (AttackButton && AbilityButton && ItemButton && LeaveButton)
	{
		AttackButton->OnClicked.AddDynamic(this, &UUserSelectionWidget::AttackButtonClicked);
		AbilityButton->OnClicked.AddDynamic(this, &UUserSelectionWidget::AbilityButtonClicked);
		ItemButton->OnClicked.AddDynamic(this, &UUserSelectionWidget::ItemButtonClicked);
		LeaveButton->OnClicked.AddDynamic(this, &UUserSelectionWidget::LeaveButtonClicked);
		ActiveArmLeftButton->OnClicked.AddDynamic(this, &UUserSelectionWidget::OnActiveArmLeftButtonClicked);
		ActiveArmRightButton->OnClicked.AddDynamic(this, &UUserSelectionWidget::OnActiveArmRightButtonClicked);
	}

	Manager = UCombatManager::GetManagerInstance(*this);
}

void UUserSelectionWidget::AttackButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(8, 1.0f, FColor::Green, TEXT("Attack Button clicked!"));

	Cast<ASnapOutOfItCharacter>(UGameplayStatics::GetPlayerPawn(this, 0))->SetAttackType(false);

	Manager->HandleAttackClicked();
	ItemMenuUI->SetVisibility(ESlateVisibility::Hidden);
	ArmSelectionUI->SetVisibility(ESlateVisibility::Hidden);
}

void UUserSelectionWidget::AbilityButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(8, 1.0f, FColor::Green, TEXT("Ability Button clicked!"));

	Cast<ASnapOutOfItCharacter>(UGameplayStatics::GetPlayerPawn(this, 0))->SetAttackType(true);

	Manager->HandleSpecialAttackClicked();
	ItemMenuUI->SetVisibility(ESlateVisibility::Hidden);
	ArmSelectionUI->SetVisibility(ESlateVisibility::Hidden);
}

void UUserSelectionWidget::ItemButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(8, 1.0f, FColor::Green, TEXT("Item Button clicked!"));

	SetSubmenuVisibility(ItemMenuUI);
}

void UUserSelectionWidget::LeaveButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(8, 1.0f, FColor::Green, TEXT("Leave Button clicked!"));

	UGameplayStatics::OpenLevel(GWorld, "Level_1");
}

void UUserSelectionWidget::OnActiveArmLeftButtonClicked()
{
	SetSubmenuVisibility(ArmSelectionUI);
	
	ArmSelectionUI->SetSelectedArmSide(ESelectionType::ST_LEFT_ARM);
}

void UUserSelectionWidget::OnActiveArmRightButtonClicked()
{
	SetSubmenuVisibility(ArmSelectionUI);
	
	ArmSelectionUI->SetSelectedArmSide(ESelectionType::ST_RIGHT_ARM);
}

void UUserSelectionWidget::SetSubmenuVisibility(UUserWidget* widget)
{
	if (widget->GetVisibility() == ESlateVisibility::Hidden) widget->SetVisibility(ESlateVisibility::Visible);
	else widget->SetVisibility(ESlateVisibility::Hidden);
}

void UUserSelectionWidget::SetHealFlaskCount(const uint32& a_count) const
{
	ItemMenuUI->SetHealFlaskCount(a_count);
}

void UUserSelectionWidget::SetArmName() const
{
	ArmSelectionUI->SetArmName();
}

void UUserSelectionWidget::SetActiveArmIcon(UTexture2D* icon, const ESelectionType& armSide) const
{
	switch (armSide)
	{
	case ESelectionType::ST_LEFT_ARM:
		ActiveArmLeftIcon->SetBrushFromTexture(icon);
		break;
	case ESelectionType::ST_RIGHT_ARM:
		ActiveArmRightIcon->SetBrushFromTexture(icon);
		break;
	default: ;
	}
}
