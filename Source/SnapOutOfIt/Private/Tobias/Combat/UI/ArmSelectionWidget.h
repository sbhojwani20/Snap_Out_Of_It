// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SnapOutOfIt/SnapOutOfItCharacter.h"
#include "ArmSelectionWidget.generated.h"

class UCombatManager;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class UArmSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

	const FString CHARACTER_DATA_TABLE_PATH = TEXT(
		"/Script/Engine.DataTable'/Game/DataTables/Shayna/DT_Character.DT_Character'");

	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetSelectedArmSide(const ESelectionType& selectionSide);

private:
	UFUNCTION()
	void OnFirstArmButtonClicked();

	UFUNCTION()
	void OnSecondArmButtonClicked();

	UFUNCTION()
	void OnThirdArmButtonClicked();

	UFUNCTION()
	void OnFourthArmButtonClicked();

	UFUNCTION()
	void OnFifthArmButtonClicked();

	UFUNCTION()
	void OnSixthArmButtonClicked();

public:
	UFUNCTION()
	void SetArmName() const;

private:
	/// <summary>
	/// The button for the first arm.
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UButton* FirstArmButton = nullptr;

	/// <summary>
	/// The button for the first arm.
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UButton* SecondArmButton = nullptr;

	/// <summary>
	/// The button for the third arm.
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UButton* ThirdArmButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* FourthArmButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* FifthArmButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* SixthArmButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FirstArmName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SecondArmName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ThirdArmName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FourthArmName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FifthArmName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SixthArmName = nullptr;

	UPROPERTY()
	TArray<UButton*> ArmButtons = TArray<UButton*>();
	
	UPROPERTY()
	UCombatManager* Manager = nullptr;

	UPROPERTY()
	ESelectionType SelectionType = ESelectionType::ST_DEFAULT;
};
