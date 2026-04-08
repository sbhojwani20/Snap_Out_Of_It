// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIManager.generated.h"

class AEnemyAI;
class StateManager;
class AWaypoint;
/**
 * 
 */
UCLASS()
class AEnemyAIManager : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	inline AEnemyAI* GetEnemy() { return Enemy; }

	UFUNCTION()
	inline ACharacter* GetPlayer() { return Player; }

	UFUNCTION()
	inline void SetEnemy(AEnemyAI* a_enemy) { Enemy = a_enemy; }
	
	UFUNCTION()
	void SetWaypoint(TArray<AWaypoint*> a_waypoint);

	UFUNCTION()
	void SetMovementSpeed(float a_fSpeed);
	
protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void Tick(float a_deltaTime) override;

private:
	StateManager* Fsm = nullptr;

	UPROPERTY()
	ACharacter* Player = nullptr;

	UPROPERTY()
	AEnemyAI* Enemy = nullptr;
};
