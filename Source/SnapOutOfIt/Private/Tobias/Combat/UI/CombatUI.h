// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatUI.generated.h"

class UTextBlock;
class UProgressBar;
class UUserSelectionWidget;
class UCombatManager;

/**
 * 
 */
UCLASS()
class UCombatUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
   	
public:
	UFUNCTION()
	void SetUserSelectionVisibility (const ESlateVisibility& a_access) const;

	UFUNCTION()
	void SetArmChargeBar(const uint8& charge);

	UFUNCTION()
	void SetPlayerHealthBarPercent(const float& a_health) const;

	UFUNCTION()
	void SetEnemyHealthBarPercent(const float& a_health) const;

	UFUNCTION()
	void SetHealFlaskCount(const uint32& a_count) const;

	UFUNCTION()
	void SetEnemyName(const FText& name) const;

	UFUNCTION()
	void SetStatusEffect(const FText& effectName) const;

	UFUNCTION()
	void SetArmName() const;

	UFUNCTION()
	void SetArmIcon(UTexture2D* icon, const ESelectionType& armSide) const;

	UFUNCTION()
	void InitializeUI();

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBarPlayer = nullptr;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBarEnemy = nullptr;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ArmChargeBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	UUserSelectionWidget* UserSelectionUI = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyName = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusEffect = nullptr;

	UPROPERTY()
	UCombatManager* Manager = nullptr;
};
