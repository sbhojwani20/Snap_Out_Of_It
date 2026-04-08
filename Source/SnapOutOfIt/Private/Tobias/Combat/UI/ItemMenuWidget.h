// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UCombatManager;

/**
 * 
 */
UCLASS()
class UItemMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HealFlaskButtonClicked();

public:
	UFUNCTION()
	void SetHealFlaskCount(const uint32& a_count) const;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* HealFlaskButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealFlaskCount = nullptr;

	UPROPERTY()
	UCombatManager* Manager = nullptr;
};
