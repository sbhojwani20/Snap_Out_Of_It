// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Notifys/SwapArmAnimNotify.h"

#include "SnapOutOfIt/SnapOutOfItCharacter.h"

void USwapArmAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Cast<ASnapOutOfItCharacter>(MeshComp->GetOwner())->SwapArms();
}
