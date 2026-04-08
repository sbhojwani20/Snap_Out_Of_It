// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Notifys/SwapArmEndAnimNotify.h"

#include "SnapOutOfIt/SnapOutOfItCharacter.h"

void USwapArmEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Cast<ASnapOutOfItCharacter>(MeshComp->GetOwner())->FinishArmSwitch();
}
