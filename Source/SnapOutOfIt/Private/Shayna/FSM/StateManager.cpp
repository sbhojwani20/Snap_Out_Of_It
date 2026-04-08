// Fill out your copyright notice in the Description page of Project Settings.


#include "Shayna/FSM/StateManager.h"
#include "Kismet/GameplayStatics.h"	
#include "Shayna/Enemy/EnemyAIManager.h"
#include "Shayna/FSM/BaseState.h"
#include "Shayna/FSM/PatrolState.h"
#include "Shayna/FSM/ChaseState.h"
#include "Shayna/FSM/AttackState.h"
#include "Shayna/Enemy/Waypoint.h"
#include "Shayna/Enemy/EnemyAI.h"

void StateManager::Initialize(void)
{
	PatrolingState = new PatrolState(this);
	ChasingState = new ChaseState(this, Manager->GetPlayer());
	AttackingState = new AttackState(this, Manager->GetPlayer());

	Transition(PatrolingState);
}

void StateManager::Tick(float a_deltaTime)
{
	if (CurrentState != nullptr) CurrentState->UpdateState(a_deltaTime);
}

void StateManager::Transition(BaseState* a_nextState)
{
	if (CurrentState != nullptr) CurrentState->ExitState();

	CurrentState = a_nextState;

	if (CurrentState != nullptr) CurrentState->EnterState();
}

void StateManager::SetWaypoints(TArray<AWaypoint*> a_waypoint)
{
	static_cast<PatrolState*>(PatrolingState)->SetWaypoints(a_waypoint);
}
