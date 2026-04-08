// Fill out your copyright notice in the Description page of Project Settings.


#include "Shayna/Enemy/EnemyAIManager.h"
#include "Shayna/FSM/StateManager.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"
#include "Shayna/Enemy/EnemyAI.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEnemyAIManager::SetWaypoint(TArray<AWaypoint*> a_waypoint)
{
	Fsm->SetWaypoints(a_waypoint);
}

void AEnemyAIManager::SetMovementSpeed(float a_fSpeed)
{
	if (Enemy && Enemy->GetCharacterMovement()) Enemy->GetCharacterMovement()->MaxWalkSpeed = a_fSpeed;
}

void AEnemyAIManager::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (Fsm == nullptr) Fsm = new StateManager(this);
	if (Fsm != nullptr) Fsm->Initialize();
}

void AEnemyAIManager::Tick(float a_deltaTime)
{
	if (Fsm != nullptr) Fsm->Tick(a_deltaTime);
}
