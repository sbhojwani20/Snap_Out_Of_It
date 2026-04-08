// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class UGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	static auto GetManagerInstance(const UObject& a_target) -> UGameManager* const;

private:
	static auto GetGameInstance(const UObject& a_target) -> UGameInstance* const;

public:
	virtual void Initialize(FSubsystemCollectionBase& a_collection) override;
	virtual void Deinitialize() override;
	
};
