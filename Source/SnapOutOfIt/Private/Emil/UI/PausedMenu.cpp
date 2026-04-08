// Fill out your copyright notice in the Description page of Project Settings.


#include "Emil/UI/PausedMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../SnapOutOfItCharacter.h"


void UPausedMenu::NativeConstruct()
{
	Super::NativeConstruct();
	ResumeButton->OnClicked.AddDynamic(this, &UPausedMenu::ResumeButtonClicked);
	RetryButton->OnClicked.AddDynamic(this, &UPausedMenu::RetryButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UPausedMenu::QuitButtonClicked);

	/*auto characterDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(*CHARACTER_DATA_PATH);
	if (characterDataTable.Succeeded()) characterData = characterDataTable.Object;

	auto enemyDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(*ENEMY_DATA_PATH);
	if (enemyDataTable.Succeeded()) enemyData = enemyDataTable.Object;*/
}
/// <summary>
/// if the buttonn is clicked close game
/// </summary>
void UPausedMenu::QuitButtonClicked()
{
	auto worldref = GetWorld();
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit; UKismetSystemLibrary::QuitGame(worldref, UGameplayStatics::GetPlayerController(worldref, 0), QuitPreference, true);
}
/// <summary>
/// if the buttonn is clicked un freze the game 
/// </summary>
void UPausedMenu::ResumeButtonClicked()
{
	const bool bIsPaused = !UGameplayStatics::IsGamePaused(GetWorld());
	UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);
	SetVisibility(ESlateVisibility::Hidden);
	//RemoveFromParent();
}

void UPausedMenu::RetryButtonClicked()
{
	characterData->AddRow(FName("GrapplingData"), FCharacterData());
	
	auto names = enemyData->GetRowNames();

	for (auto name : names)
	{
		enemyData->RemoveRow(name);
	}

	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}
