// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shayna/Enemy/EnemyData.h"
#include "PausedMenu.generated.h"

class UButton;
class UDataTable;

/**
 * 
 */
UCLASS()
class UPausedMenu : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	
	const FString CHARACTER_DATA_PATH = TEXT("/Script/Engine.DataTable'/Game/DataTables/Shayna/DT_Character.DT_Character'");
	const FString ENEMY_DATA_PATH = TEXT("/Script/Engine.DataTable'/Game/DataTables/Shayna/DT_Enemy.DT_Enemy'");

	UFUNCTION()
	void QuitButtonClicked();

	UFUNCTION()
	void ResumeButtonClicked();

	UFUNCTION()
	void RetryButtonClicked();


private: 
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* RetryButton = nullptr;
	
	UPROPERTY()
	UDataTable* characterData = nullptr;

	UPROPERTY()
	UDataTable* enemyData = nullptr;
};
