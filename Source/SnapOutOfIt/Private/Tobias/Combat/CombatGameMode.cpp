// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/CombatGameMode.h"
#include "../SnapOutOfItCharacter.h"
#include "Emil/UI/LevelHUD.h"
#include "Tobias/Combat/CombatPlayerController.h"

ACombatGameMode::ACombatGameMode()
{
	DefaultPawnClass = ASnapOutOfItCharacter::StaticClass();

	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	// if (PlayerPawnBPClass.Succeeded())
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }

	HUDClass = ALevelHUD::StaticClass();
	PlayerControllerClass = ACombatPlayerController::StaticClass();
}

void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();
}
