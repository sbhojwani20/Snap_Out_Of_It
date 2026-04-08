// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Notifys/AttackAnimNotify.h"
#include "Tobias/Combat/CombatEntity.h"

void UAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Cast<ACombatEntity>(MeshComp->GetOwner())->ApplyDamageToTarget();
}
