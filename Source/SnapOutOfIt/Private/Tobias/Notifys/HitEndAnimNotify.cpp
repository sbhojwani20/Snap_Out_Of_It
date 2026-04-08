// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Notifys/HitEndAnimNotify.h"

#include "SnapOutOfIt/SnapOutOfItCharacter.h"
#include "Tobias/Combat/CombatEntity.h"
#include "Tobias/Combat/CombatManager.h"

void UHitEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	auto entity = Cast<ACombatEntity>(MeshComp->GetAttachParentActor());

	if (entity->CheckDeathConditions())
	{
		UCombatManager::GetManagerInstance(*MeshComp)->SetEnemy(nullptr);
		
		return;
	}
	if (entity->IsA<ASnapOutOfItCharacter>()) Cast<ASnapOutOfItCharacter>(entity)->CheckStatusEffect();
	else entity->CheckStatusEffect();
}
