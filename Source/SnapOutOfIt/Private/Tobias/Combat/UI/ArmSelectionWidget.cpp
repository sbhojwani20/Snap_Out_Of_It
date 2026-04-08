// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/UI/ArmSelectionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Shayna/Character/CharacterData.h"
#include "SnapOutOfIt/SnapOutOfItCharacter.h"
#include "Tobias/Combat/CombatEntity.h"
#include "Tobias/Combat/CombatManager.h"

void UArmSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (FirstArmButton && SecondArmButton && ThirdArmButton && FourthArmButton && FifthArmButton && SixthArmButton)
	{
		FirstArmButton->OnClicked.AddDynamic(this, &UArmSelectionWidget::OnFirstArmButtonClicked);
		SecondArmButton->OnClicked.AddDynamic(this, &UArmSelectionWidget::OnSecondArmButtonClicked);
		ThirdArmButton->OnClicked.AddDynamic(this, &UArmSelectionWidget::OnThirdArmButtonClicked);
		FourthArmButton->OnClicked.AddDynamic(this, &UArmSelectionWidget::OnFourthArmButtonClicked);
		FifthArmButton->OnClicked.AddDynamic(this, &UArmSelectionWidget::OnFifthArmButtonClicked);
		SixthArmButton->OnClicked.AddDynamic(this, &UArmSelectionWidget::OnSixthArmButtonClicked);
	}

	Manager = UCombatManager::GetManagerInstance(*this);

	FirstArmButton->SetVisibility(ESlateVisibility::Hidden);
	SecondArmButton->SetVisibility(ESlateVisibility::Hidden);
	ThirdArmButton->SetVisibility(ESlateVisibility::Hidden);
	FourthArmButton->SetVisibility(ESlateVisibility::Hidden);
	FifthArmButton->SetVisibility(ESlateVisibility::Hidden);
	SixthArmButton->SetVisibility(ESlateVisibility::Hidden);

	SetVisibility(ESlateVisibility::Hidden);
}

void UArmSelectionWidget::SetSelectedArmSide(const ESelectionType& armPosition)
{
	SelectionType = armPosition;
}

void UArmSelectionWidget::OnFirstArmButtonClicked()
{
	Cast<ASnapOutOfItCharacter>(Manager->GetPlayer())->HandleArmSwitchClicked(5, SelectionType);
	SetVisibility(ESlateVisibility::Hidden);
}

void UArmSelectionWidget::OnSecondArmButtonClicked()
{
	Cast<ASnapOutOfItCharacter>(Manager->GetPlayer())->HandleArmSwitchClicked(4, SelectionType);
	SetVisibility(ESlateVisibility::Hidden);
}

void UArmSelectionWidget::OnThirdArmButtonClicked()
{
	Cast<ASnapOutOfItCharacter>(Manager->GetPlayer())->HandleArmSwitchClicked(3, SelectionType);
	SetVisibility(ESlateVisibility::Hidden);
}

void UArmSelectionWidget::OnFourthArmButtonClicked()
{
	Cast<ASnapOutOfItCharacter>(Manager->GetPlayer())->HandleArmSwitchClicked(2, SelectionType);
	SetVisibility(ESlateVisibility::Hidden);
}

void UArmSelectionWidget::OnFifthArmButtonClicked()
{
	Cast<ASnapOutOfItCharacter>(Manager->GetPlayer())->HandleArmSwitchClicked(1, SelectionType);
	SetVisibility(ESlateVisibility::Hidden);
}

void UArmSelectionWidget::OnSixthArmButtonClicked()
{
	Cast<ASnapOutOfItCharacter>(Manager->GetPlayer())->HandleArmSwitchClicked(0, SelectionType);
	SetVisibility(ESlateVisibility::Hidden);
}

void UArmSelectionWidget::SetArmName() const
{
	FirstArmButton->SetVisibility(ESlateVisibility::Hidden);
	SecondArmButton->SetVisibility(ESlateVisibility::Hidden);
	ThirdArmButton->SetVisibility(ESlateVisibility::Hidden);
	FourthArmButton->SetVisibility(ESlateVisibility::Hidden);
	FifthArmButton->SetVisibility(ESlateVisibility::Hidden);
	SixthArmButton->SetVisibility(ESlateVisibility::Hidden);
	
	if (!Manager->GetPlayer()) return;
	
	auto armInventory = Cast<ASnapOutOfItCharacter>(Manager->GetPlayer())->GetCharacterData().CollectedArms;

	for (int32 i = 0; i < armInventory.Num(); i++)
	{
		switch (i)
		{
		case 0:
			if (armInventory[i].ArmType == EArmType::ET_DEFAULT) SixthArmButton->
				SetVisibility(ESlateVisibility::Hidden);
			else
			{
				SixthArmButton->SetVisibility(ESlateVisibility::Visible);
				SixthArmName->SetText(armInventory[i].ArmName);
			}
			break;
		case 1:
			if (armInventory[i].ArmType == EArmType::ET_DEFAULT) FifthArmButton->
				SetVisibility(ESlateVisibility::Hidden);
			else
			{
				FifthArmButton->SetVisibility(ESlateVisibility::Visible);
				FifthArmName->SetText(armInventory[i].ArmName);
			}
			break;
		case 2:
			if (armInventory[i].ArmType == EArmType::ET_DEFAULT) FourthArmButton->
				SetVisibility(ESlateVisibility::Hidden);
			else
			{
				FourthArmButton->SetVisibility(ESlateVisibility::Visible);
				FourthArmName->SetText(armInventory[i].ArmName);
			}
			break;
		case 3:
			if (armInventory[i].ArmType == EArmType::ET_DEFAULT) ThirdArmButton->
				SetVisibility(ESlateVisibility::Hidden);
			else
			{
				ThirdArmButton->SetVisibility(ESlateVisibility::Visible);
				ThirdArmName->SetText(armInventory[i].ArmName);
			}
			break;
		case 4:
			if (armInventory[i].ArmType == EArmType::ET_DEFAULT) SecondArmButton->
				SetVisibility(ESlateVisibility::Hidden);
			else
			{
				SecondArmButton->SetVisibility(ESlateVisibility::Visible);
				SecondArmName->SetText(armInventory[i].ArmName);
			}
			break;
		case 5:
			if (armInventory[i].ArmType == EArmType::ET_DEFAULT) FirstArmButton->
				SetVisibility(ESlateVisibility::Hidden);
			else
			{
				FirstArmButton->SetVisibility(ESlateVisibility::Visible);
				FirstArmName->SetText(armInventory[i].ArmName);
			}
			break ;
		default: ;
		}
	}
}
