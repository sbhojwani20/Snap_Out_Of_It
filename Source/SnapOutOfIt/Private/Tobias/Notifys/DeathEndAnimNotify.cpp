// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Notifys/DeathEndAnimNotify.h"
#include "Shayna/Enemy/EnemyAI.h"
#include "../SnapOutOfItCharacter.h"

void UDeathEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto entity = Cast<ACombatEntity>(MeshComp->GetAttachParentActor()); 
	
	if (entity->IsA<AEnemyAI>()) Cast<AEnemyAI>(entity)->HandleDeath();
	else Cast<ASnapOutOfItCharacter>(MeshComp->GetAttachParentActor())->HandleDeath();
}
