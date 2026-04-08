// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UPickUpUI.generated.h"
class UTextBlock;
/**
 * 
 */
UCLASS()
class UUPickUpUI : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
public:
	void SetArmName(const FString& a_sArmName);
private:
	 FString testArmName = FString(TEXT("No Item Name"));
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ArmName = nullptr;
};
