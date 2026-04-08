// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Notifys/AttackEndAnimNotify.h"
#include "Tobias/Combat/CombatEntity.h"

void UAttackEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Cast<ACombatEntity>(MeshComp->GetAttachParentActor())->EndTurn();
}
