// Fill out your copyright notice in the Description page of Project Settings.


#include "Emil/UI/LevelHUD.h"
#include "Blueprint/UserWidget.h"
#include "Emil/UI/UPickUpUI.h"
#include "Emil/UI/PausedMenu.h"
#include "Kismet/GameplayStatics.h"


//Cartion (what is needed)
ALevelHUD::ALevelHUD()
{
    //PickUpWidgetClass = ConstructorHelpers::FClassFinder<UUserWidget>(*PICK_UP_UI_PATH).Class;
    auto pickup = ConstructorHelpers::FClassFinder<UUserWidget>(*PICK_UP_UI_PATH);
    if (pickup.Succeeded()) PickUpWidgetClass = pickup.Class;

    //PausedMenuWidgetClass = ConstructorHelpers::FClassFinder<UUserWidget>(*PAUSED_MENU_UI_PATH).Class;
    auto paused = ConstructorHelpers::FClassFinder<UUserWidget>(*PAUSED_MENU_UI_PATH);
    if (paused.Succeeded()) PausedMenuWidgetClass = paused.Class;
    else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("no Class in ctor"));

    DeathScreenClass  = ConstructorHelpers::FClassFinder<UUserWidget>(*WBP_DEATH_SCREEN_PATH).Class;
}

void ALevelHUD::BeginPlay()
{
    if (PickUpWidgetClass) PickUpUI = CreateWidget<UUPickUpUI>(GetWorld()->GetFirstPlayerController(), PickUpWidgetClass);
    if (PausedMenuWidgetClass) PausedMenuUI = CreateWidget<UPausedMenu>(GetWorld()->GetFirstPlayerController(), PausedMenuWidgetClass);
    else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("no Class"));
    if (DeathScreenClass) DeathScreenUI = CreateWidget<UPausedMenu>(GetWorld()->GetFirstPlayerController(), DeathScreenClass);

    if (PausedMenuUI) 
    {
        PausedMenuUI->AddToViewport(); 
        PausedMenuUI->SetVisibility(ESlateVisibility::Hidden);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("PauseMenu added to viewport"));
    }
}
/// <summary>
/// Put the ui on the screen
/// </summary>
void ALevelHUD::ShowPickUpUI()
{
    if (PickUpUI) PickUpUI->AddToViewport();
}
/// <summary>
/// Hide the pick up ui
/// </summary>
void ALevelHUD::HidePickUpUI()
{
    if (PickUpUI) PickUpUI->RemoveFromParent();
}

/// <summary>
/// Activation of the pause ui
/// </summary>
void ALevelHUD::ShowPausedMenu()
{
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

    PausedMenuUI->SetVisibility(ESlateVisibility::Visible);  
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ALevelHUD::HidePausedMenu()
{
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

    PausedMenuUI->SetVisibility(ESlateVisibility::Hidden);
    UGameplayStatics::SetGamePaused(GetWorld(), false);
}

#pragma region Code from Tobias
void ALevelHUD::ShowDeathScreen()
{
    if (DeathScreenUI) DeathScreenUI->AddToViewport();
}

/// <summary>
/// changing the arm name in the ui 
/// </summary>
/// <param name="a_sArmName"></param>
void ALevelHUD::SetArmName(const FString& a_sArmName)
{
    if (PickUpUI) PickUpUI->SetArmName(a_sArmName);
}


