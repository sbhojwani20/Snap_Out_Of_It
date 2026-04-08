// Fill out your copyright notice in the Description page of Project Settings.


#include "Shayna/World/WorldManager.h"
#include "Shayna/Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Tobias/Combat/CombatManager.h"
#include "../SnapOutOfItCharacter.h"


AWorldManager::AWorldManager() 
{
	auto sound = ConstructorHelpers::FObjectFinder<USoundWave>(*AMBIENT_AUDIO_PATH);
	BackgroundMusic = sound.Object;
}

void AWorldManager::BeginPlay()
{
	AudioManager = UAudioManager::GetManagerInstance(*this);

	AudioManager->PlayBackgroundMusic(BackgroundMusic, this);
}
