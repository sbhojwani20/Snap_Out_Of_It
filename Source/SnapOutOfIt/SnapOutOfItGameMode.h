// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Shayna/World/WorldData.h"
#include "SnapOutOfItGameMode.generated.h"

class UCombatManager;

UCLASS(minimalapi)
class ASnapOutOfItGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASnapOutOfItGameMode();

private:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	UCombatManager* CombatManager = nullptr;
};



