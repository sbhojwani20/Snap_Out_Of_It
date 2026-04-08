// Fill out your copyright notice in the Description page of Project Settings.


#include "Shayna/FSM/PatrolState.h"
#include "Shayna/Enemy/EnemyAIManager.h"
#include "Shayna/Enemy/EnemyAI.h"
#include "Shayna/Enemy/Waypoint.h"


void PatrolState::EnterState(void)
{
	if (Waypoints.Num() > 0)
	{
		if (CurrentWaypoint < 0) CurrentWaypoint = 0;
		UpdateDestination();
	}
}

void PatrolState::UpdateState(float a_deltaTime)
{
	if (CurrentWaypoint < 0) return;
	auto enemy = Fsm->GetAIManager()->GetEnemy();

	if (!IsWalking)
	{
		if (enemy->GetStats().WalkAnimSequ && enemy->GetMesh()) enemy->GetMesh()->PlayAnimation(enemy->GetStats().WalkAnimSequ.Get(), true);

		IsWalking =true;
	}

	if (enemy->GetCollidedWithPlayer())
	{
		enemy->SetCollidedWithPlayer(false);
		Fsm->Transition(Fsm->GetChaseState());
		return;
	}

	/*if (enemy->GetCollidedWithPlayer() && enemy->GetType() == E_ENTITY_TYPE::ET_BUTCHER)
	{
		enemy->SetCollidedWithPlayer(false);
		Fsm->Transition(Fsm->GetAttackState());
		return;
	}
	else
	{
		enemy->SetCollidedWithPlayer(false);
		Fsm->Transition(Fsm->GetChaseState());
		return;
	}*/

	if (CurrentWaypoint >= 0 && CurrentWaypoint < Waypoints.Num())
	{
		auto waypointDist = FVector::Dist(Waypoints[CurrentWaypoint]->GetActorLocation(), Fsm->GetAIManager()->GetEnemy()->GetActorLocation());

		if (waypointDist < 100.0f)
		{
			if (!IsIdling)
			{
				Fsm->GetAIManager()->SetMovementSpeed(0.0f);

				if (enemy->GetStats().IdleAnimSequ && enemy->GetMesh()) enemy->GetMesh()->PlayAnimation(enemy->GetStats().IdleAnimSequ.Get(), true);
				
				IsIdling = true;
				idleTimer = 0.0f;
			}
				
			idleTimer += a_deltaTime;

			if (idleTimer >= idleDuration)
			{
				CurrentWaypoint++;
				if (CurrentWaypoint >= Waypoints.Num()) CurrentWaypoint = 0;

				Fsm->GetAIManager()->SetMovementSpeed(75.0f);

				if (enemy->GetStats().WalkAnimSequ && enemy->GetMesh()) enemy->GetMesh()->PlayAnimation(enemy->GetStats().WalkAnimSequ.Get(), true);

				UpdateDestination();
				
				IsIdling = false;
			}
		}
		else UpdateDestination();
	}
}

void PatrolState::SetWaypoints(TArray<AWaypoint*> a_waypoint)
{
	Waypoints = a_waypoint;
	if (Waypoints.Num() > 0)
	{
		CurrentWaypoint = 0;
		UpdateDestination();
	}
}

void PatrolState::UpdateDestination(void)
{
	auto enemy = Fsm->GetAIManager()->GetEnemy();

	if (enemy)
	{ 
		Fsm->GetAIManager()->SetMovementSpeed(75.0f);
	
		Fsm->GetAIManager()->MoveToLocation(Waypoints[CurrentWaypoint]->GetActorLocation());
	}
}
