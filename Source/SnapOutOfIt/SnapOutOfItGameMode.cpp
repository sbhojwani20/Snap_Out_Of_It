// Copyright Epic Games, Inc. All Rights Reserved.

#include "SnapOutOfItGameMode.h"

#include "SnapOutOfItCharacter.h"
#include "Emil/UI/LevelHUD.h"
#include "Shayna/World/WorldData.h"
#include "Tobias/Combat/CombatManager.h"
#include "UObject/ConstructorHelpers.h"

ASnapOutOfItGameMode::ASnapOutOfItGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));

	if (PlayerPawnBPClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		//DefaultPawnClass = ASnapOutOfItCharacter::StaticClass();
		HUDClass = ALevelHUD::StaticClass();
	}	
}

void ASnapOutOfItGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	CombatManager = UCombatManager::GetManagerInstance(*this);

	CombatManager->SetPlayerStart(FindPlayerStart(GetWorld()->GetFirstPlayerController()));
}
