// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Sound/SoundBase.h"
#include "SFXNotify.generated.h"

class UAudioManager;
/**
 * 
 */
UCLASS()
class USFXNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SoundToPlay;

	UFUNCTION()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private: 
	UPROPERTY()
	UAudioManager* AudioManager = nullptr;
};
