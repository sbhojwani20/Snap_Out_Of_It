// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"

class StateManager;
class ACharacter;
/**
 * 
 */
class AttackState : public BaseState
{
public:
	AttackState(StateManager* a_fsm, ACharacter* a_toAttack) : Fsm(a_fsm), ActorToAttack(a_toAttack) {}
	inline ~AttackState() {}

	virtual void EnterState(void) override;

private:		
	StateManager* Fsm = nullptr;
	ACharacter* ActorToAttack = nullptr;
};
