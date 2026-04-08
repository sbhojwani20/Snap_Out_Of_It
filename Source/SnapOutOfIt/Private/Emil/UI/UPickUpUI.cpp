// Fill out your copyright notice in the Description page of Project Settings.


#include "Emil/UI/UPickUpUI.h"
#include "Components/TextBlock.h"

void UUPickUpUI::NativeConstruct()
{
	Super::NativeConstruct();
	ArmName->SetText(FText::FromString(testArmName));
}

void UUPickUpUI::SetArmName(const FString& a_sArmName)
{
	//testArmName = a_sArmName;
	ArmName->SetText(FText::FromString(a_sArmName));
}
