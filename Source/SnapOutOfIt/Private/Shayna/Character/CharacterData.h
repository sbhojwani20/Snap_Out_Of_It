// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Emil/ArmData.h"
#include "CharacterData.generated.h"

USTRUCT(BlueprintType)
struct FArmInventorySlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arm Info")
	EArmType ArmType = EArmType::ET_DEFAULT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms Info")
	FText ArmName = FText();
};

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grappling Hook")
	bool bGrappleActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	EArmType ActiveArmLeft = EArmType::ET_DEFAULT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arms")
	EArmType ActiveArmRight = EArmType::ET_MELTED_ARM;

	UPROPERTY(EditAnywhere, Category = "Arms")
	TArray<FArmInventorySlotData> CollectedArms = TArray<FArmInventorySlotData>();
};
