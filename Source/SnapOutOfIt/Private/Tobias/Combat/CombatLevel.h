// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CombatLevel.generated.h"

class UAudioManager;
class ACombatEntity;
/**
 *
 */
UCLASS()
class ACombatLevel : public ALevelScriptActor
{
	GENERATED_BODY()

private:
	ACombatLevel();

	void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCombatUI> CombatUIObject = {};

	UPROPERTY()
	class UCombatUI* CombatUI = nullptr;

#pragma region Shaynas Code

	const FString MINIONS_AUDIO_PATH = TEXT("/Script/Engine.SoundWave'/Game/Assets/Audios/Levels/Minions_Battle.Minions_Battle'");
	const FString BOSS_AUDIO_PATH = TEXT("/Script/Engine.SoundWave'/Game/Assets/Audios/Levels/ReclaimWhatsYours_BossBattle_VersFinal_1_.ReclaimWhatsYours_BossBattle_VersFinal_1_'");

	UPROPERTY()
	UAudioManager* AudioManager = nullptr;

	UPROPERTY()
	USoundWave* BossBackgroundMusic = nullptr;

	UPROPERTY()
	USoundWave* MinionsBackgroundMusic = nullptr;
#pragma endregion 
};
