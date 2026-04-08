// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Notifys/SpecialAttackAnimNotify.h"
#include "SnapOutOfIt/SnapOutOfItCharacter.h"

void USpecialAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Cast<ASnapOutOfItCharacter>(MeshComp->GetOwner())->ApplySpecialDamageToTarget();
}
