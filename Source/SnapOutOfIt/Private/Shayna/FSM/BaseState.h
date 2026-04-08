// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shayna/FSM/StateManager.h"

class StateManager;

/**
 * 
 */
class BaseState
{
public:
	inline BaseState() {}
	inline BaseState(FString a_name, StateManager* a_fsm) : Fsm(a_fsm) {}
	inline virtual ~BaseState() {}

	inline virtual void EnterState(void) {}
	inline virtual void UpdateState(float a_deltaTime) {}
	inline virtual void ExitState(void) {}

protected:
	FString Name = TEXT("");
	StateManager* Fsm = nullptr;
};
