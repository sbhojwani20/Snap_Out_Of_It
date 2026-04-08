// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldData.generated.h"

USTRUCT(BlueprintType)
struct FWorldData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FRotator PlayerRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
	FName LastLevelName = FName(TEXT(""));
};