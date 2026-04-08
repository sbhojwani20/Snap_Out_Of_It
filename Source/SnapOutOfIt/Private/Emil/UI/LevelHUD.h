// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LevelHUD.generated.h"

class ULoadingScreenUI;
class UUPickUpUI;
class UPausedMenu;
/**
 * 
 */
UCLASS()
class ALevelHUD : public AHUD
{
	GENERATED_BODY()

#pragma region Code from Tobias
	const FString WBP_DEATH_SCREEN_PATH = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DewiAssets/Blueprints/WBP_DeathScreen.WBP_DeathScreen_C'");
#pragma endregion

public:
	ALevelHUD();
	virtual void BeginPlay() override;
	void ShowPickUpUI();
	void HidePickUpUI();
	void ShowPausedMenu();
	void HidePausedMenu();

#pragma region Code from Tobias
	UFUNCTION()
	void ShowDeathScreen();
#pragma endregion
	
	void SetArmName(const FString& a_sArmName);

	UPausedMenu* GetPausedMenu(void) { return PausedMenuUI; }

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Pick Up Widget Class", Category = "GUI", AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> PickUpWidgetClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Paused Widget Class", Category = "GUI", AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> PausedMenuWidgetClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Pick Up Text Ui", Category = "GUI", AllowPrivateAccess = true))
	UUPickUpUI* PickUpUI = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Paused Screen", Category = "GUI", AllowPrivateAccess = true))
	UPausedMenu* PausedMenuUI = nullptr;

#pragma region Code from Tobias
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Death Screen Class", Category = "GUI", AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> DeathScreenClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Death Screen UI", Category = "GUI", AllowPrivateAccess = true))
	UUserWidget* DeathScreenUI = nullptr;
#pragma endregion

	UPROPERTY()
	ALevelHUD* PickUpWidget = nullptr;

	bool isGamePaused = false;
	
	const FString PICK_UP_UI_PATH = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Emil/PickUpUI/WBP_PickUpUI.WBP_PickUpUI_C'");
	const FString PAUSED_MENU_UI_PATH = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Emil/PauseMenuUI/WBP_PausedMenu.WBP_PausedMenu_C'");
};
