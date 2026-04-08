// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Notifys/HealAnimNotify.h"
#include "SnapOutOfIt/SnapOutOfItCharacter.h"

void UHealAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Cast<ASnapOutOfItCharacter>(MeshComp->GetAttachParentActor())->HealDamage(EHealType::HT_HEAL_FLASK);
}
