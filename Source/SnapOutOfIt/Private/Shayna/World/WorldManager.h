// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "WorldManager.generated.h"

class UAudioManager;
class USoundWave;
/**
 * 
 */
UCLASS()
class AWorldManager : public ALevelScriptActor
{
	GENERATED_BODY()

private:
	AWorldManager();

	virtual void BeginPlay() override;

	const FString AMBIENT_AUDIO_PATH = TEXT("/Script/Engine.SoundWave'/Game/Assets/Audios/Levels/Ambient_Music.Ambient_Music'");

	UPROPERTY(EditAnywhere, Category = "Level Map")
	TSoftObjectPtr<UWorld> LevelMap = {};
	
	UPROPERTY()
	UAudioManager* AudioManager = nullptr;

	UPROPERTY()
	USoundWave* BackgroundMusic = nullptr;
	
};
