// Fill out your copyright notice in the Description page of Project Settings.


#include "Shayna/FSM/ChaseState.h"
#include "Shayna/Enemy/EnemyAIManager.h"
#include "Shayna/Enemy/EnemyAI.h"


void ChaseState::UpdateState(float a_deltaTime)
{
	if (!IsChasing)
	{
		auto enemy = Fsm->GetAIManager()->GetEnemy();

		Fsm->GetAIManager()->SetMovementSpeed(200.0f);

		if (enemy->GetStats().WalkAnimSequ && enemy->GetMesh()) enemy->GetMesh()->PlayAnimation(enemy->GetStats().WalkAnimSequ.Get(), true);

		IsChasing = true;
	}

	Fsm->GetAIManager()->MoveToLocation(ActorToChase->GetActorLocation());

	auto chaseDist = FVector::Dist(Fsm->GetAIManager()->GetEnemy()->GetActorLocation(), ActorToChase->GetActorLocation());

	if (chaseDist < 200)
	{
		Fsm->Transition(Fsm->GetAttackState());
		return;
	}
}
