// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"

class StateManager;
class AWaypoint;
/**
 * 
 */
class PatrolState : public BaseState
{
public:
	PatrolState(StateManager* a_fsm) : Fsm(a_fsm) {}
	inline ~PatrolState() {}

	virtual void EnterState(void) override;
	virtual void UpdateState(float a_deltaTime) override;

	void SetWaypoints(TArray<AWaypoint*> a_waypoint);

private:
	void UpdateDestination(void);

	StateManager* Fsm = nullptr;
	TArray<AWaypoint*> Waypoints;

	int CurrentWaypoint = -1;
	float idleTimer = 0.0f; 
	float idleDuration = 2.0f; 
	bool IsIdling = false;
	bool IsWalking = false;
};
