// Fill out your copyright notice in the Description page of Project Settings.


#include "Shayna/Audio/SFXNotify.h"
#include "Shayna/Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void USFXNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!SoundToPlay || !MeshComp) return;

	AudioManager->PlaySFX(SoundToPlay, MeshComp->GetWorld(), MeshComp->GetComponentLocation());
}
