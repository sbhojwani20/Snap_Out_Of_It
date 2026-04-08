// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class BaseState;
class AWaypoint;
class AEnemyAIManager;
/**
 * 
 */
class StateManager
{
public:
	inline StateManager() {}
	inline StateManager(AEnemyAIManager* a_manager) : Manager(a_manager) { }

	inline virtual ~StateManager() {}

	virtual void Initialize(void);
	virtual void Tick(float a_deltaTime);
	void Transition(BaseState* a_nextState);

	AEnemyAIManager* GetAIManager(void) { return Manager; }
	BaseState* GetPatrolState(void) { return PatrolingState; }
	BaseState* GetChaseState(void) { return ChasingState; }
	BaseState* GetAttackState(void) { return AttackingState; }

	void SetWaypoints(TArray<AWaypoint*> a_waypoint);

protected:
	AEnemyAIManager* Manager = nullptr;
	BaseState* CurrentState = nullptr;
	BaseState* PatrolingState = nullptr;
	BaseState* ChasingState = nullptr;
	BaseState* AttackingState = nullptr;
};
