// Fill out your copyright notice in the Description page of Project Settings.

#include "Shayna/Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


auto UAudioManager::GetManagerInstance(const UObject& a_target) -> UAudioManager* const
{
	auto gameInstance = GetGameInstance(a_target);

	if (gameInstance) return gameInstance->GetSubsystem<UAudioManager>();

	return nullptr;
}

auto UAudioManager::GetGameInstance(const UObject& a_target) -> UGameInstance* const
{
    if (a_target.GetWorld() && a_target.GetWorld()->GetGameInstance())
    {
        return UGameplayStatics::GetGameInstance(a_target.GetWorld());
    }
    else if (a_target.IsA(AActor::StaticClass()))
    {
        if (Cast<AActor>(&a_target)->GetGameInstance()) return GEngine->GetWorld()->GetGameInstance();
    }

    return nullptr;
}

void UAudioManager::PlayBackgroundMusic(USoundBase* a_music, AActor* a_worldActor)
{
    AudioComponent = UGameplayStatics::SpawnSound2D(a_worldActor, a_music, 1.0f, 1.0f, 0.0f, nullptr, false, false);
}

void UAudioManager::PlaySFX(USoundBase* a_sound, UObject* a_worldObject, FVector a_fLocation)
{
    UGameplayStatics::SpawnSoundAtLocation(a_worldObject, a_sound, a_fLocation, FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
}

