// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioManager.generated.h"

class UAudioComponent;
/**
 * 
 */
UCLASS()
class UAudioManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static auto GetManagerInstance(const UObject& a_target) -> UAudioManager* const;
	virtual void Initialize(FSubsystemCollectionBase& a_collection) {};
	virtual void Finalize() {};

	UFUNCTION()
	void PlayBackgroundMusic(USoundBase* a_music, AActor* a_worldActor);

	UFUNCTION()
	void PlaySFX(USoundBase* a_sound, UObject* a_worldObject, FVector a_fLocation);

private:
	static auto GetGameInstance(const UObject& a_target) -> UGameInstance* const;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY()
	UAudioComponent* SFXComponent = nullptr;
};
