	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"

class StateManager;
class ACharacter;
/**
 * 
 */
class ChaseState : public BaseState
{
public:
	ChaseState(StateManager* a_fsm, ACharacter* a_toChase) : Fsm(a_fsm), ActorToChase(a_toChase) {}
	inline ~ChaseState() {}

	virtual void UpdateState(float a_deltaTime) override;
		
private:
	StateManager* Fsm = nullptr;
	ACharacter* ActorToChase = nullptr;
	bool IsChasing = false;
};
