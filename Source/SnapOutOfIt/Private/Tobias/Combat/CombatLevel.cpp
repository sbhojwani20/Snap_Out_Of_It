// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/CombatLevel.h"
#include "Tobias/Combat/UI/CombatUI.h"
#include "Tobias/Combat/CombatManager.h"
#include "Shayna/Audio/AudioManager.h"
#include "Tobias/Combat/CombatEntity.h"
#include "Kismet/GameplayStatics.h"
#include "../SnapOutOfItCharacter.h"

#pragma region Shaynas Code
ACombatLevel::ACombatLevel()
{
	auto MinionsSound = ConstructorHelpers::FObjectFinder<USoundWave>(*MINIONS_AUDIO_PATH);
	MinionsBackgroundMusic = MinionsSound.Object;

	auto BossSound = ConstructorHelpers::FObjectFinder<USoundWave>(*BOSS_AUDIO_PATH);
	BossBackgroundMusic = BossSound.Object;

	
}
#pragma endregion

void ACombatLevel::BeginPlay()
{
	if (CombatUIObject)
	{
		CombatUI = CreateWidget<UCombatUI>(GetWorld(), CombatUIObject);

		if (CombatUI)
		{
			CombatUI->AddToViewport();
		}
	}
	
#pragma region Shaynas Code
	AudioManager = UAudioManager::GetManagerInstance(*this);

	if (UCombatManager::GetManagerInstance(*this)->GetEncounter() == EEntityType::ET_ENEMY_A || 
		UCombatManager::GetManagerInstance(*this)->GetEncounter() == EEntityType::ET_ENEMY_B || 
		UCombatManager::GetManagerInstance(*this)->GetEncounter() == EEntityType::ET_ENEMY_C) AudioManager->PlayBackgroundMusic(MinionsBackgroundMusic, this);
	
	else if (UCombatManager::GetManagerInstance(*this)->GetEncounter() == EEntityType::ET_BUTCHER ||
			UCombatManager::GetManagerInstance(*this)->GetEncounter() == EEntityType::ET_FINAL_BOSS) AudioManager->PlayBackgroundMusic(BossBackgroundMusic, this);
#pragma endregion
}
