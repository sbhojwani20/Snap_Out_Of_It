// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "UserSelectionWidget.generated.h"

class UArmSelectionWidget;
class UButton;
class UItemMenuWidget;
class ACombatGameState;
class UCombatManager;
/**
 *
 */
UCLASS()
class UUserSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	// Functions
	UFUNCTION()
	void AttackButtonClicked();

	UFUNCTION()
	void AbilityButtonClicked();

	UFUNCTION()
	void ItemButtonClicked();

	UFUNCTION()
	void LeaveButtonClicked();

	UFUNCTION()
	void OnActiveArmLeftButtonClicked();

	UFUNCTION()
	void OnActiveArmRightButtonClicked();

	UFUNCTION()
	void SetSubmenuVisibility(UUserWidget* widget);
public:
	UFUNCTION()
	void SetHealFlaskCount(const uint32& a_count) const;

	UFUNCTION()
	void SetArmName() const;

	UFUNCTION()
	void SetActiveArmIcon(UTexture2D* icon, const ESelectionType& armSide) const;

private:
	// UI Elements
	UPROPERTY(meta = (BindWidget))
	UButton* AttackButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* AbilityButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* LeaveButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* ActiveArmLeftButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* ActiveArmRightButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* ActiveArmLeftIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* ActiveArmRightIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	UItemMenuWidget* ItemMenuUI = nullptr;

	UPROPERTY(meta = (BindWidget))
	UArmSelectionWidget* ArmSelectionUI = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCombatManager* Manager = nullptr;
};
