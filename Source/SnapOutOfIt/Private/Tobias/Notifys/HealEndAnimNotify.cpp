// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Notifys/HealEndAnimNotify.h"
#include "Tobias/Combat/CombatEntity.h"

void UHealEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Cast<ACombatEntity>(MeshComp->GetAttachParentActor())->EndTurn();
}
