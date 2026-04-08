// Fill out your copyright notice in the Description page of Project Settings.


#include "Shayna/FSM/AttackState.h"
#include "Kismet/GameplayStatics.h"
#include "Shayna/FSM/StateManager.h"
#include "Shayna/Enemy/EnemyAIManager.h"
#include "Engine/World.h"
#include "Shayna/Enemy/EnemyAI.h"
#include "Tobias/Combat/CombatManager.h"
#include "../SnapOutOfItCharacter.h"

void AttackState::EnterState()
{
    if (auto pawn = GWorld->GetFirstPlayerController()->GetPawn(); pawn->IsA< ASnapOutOfItCharacter>())
    {
        if (auto playerCharacter = Cast< ASnapOutOfItCharacter>(pawn); playerCharacter)
        {
            if (UCombatManager* CombatManager = UCombatManager::GetManagerInstance(*playerCharacter))
            {
                // set player start position
                playerCharacter->SetLastPlayerLocation(playerCharacter->GetActorLocation());

                playerCharacter->SetLastPlayerRotation(playerCharacter->GetActorRotation());

                // save current level
                FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GWorld));
                CombatManager->SetLastLevel(CurrentLevelName);
            }
        }
    }

    Cast<ASnapOutOfItCharacter>(Fsm->GetAIManager()->GetPlayer())->SaveLastPosition();

	auto enemy = Fsm->GetAIManager()->GetEnemy();

    enemy->GiveEnemyDataToManager();

	Fsm->GetAIManager()->SetMovementSpeed(300.0f);

	if (enemy->GetStats().AttackLeftAnimSequ && enemy->GetMesh()) enemy->GetMesh()->PlayAnimation(enemy->GetStats().AttackRightAnimSequ.Get(), true);

	UGameplayStatics::OpenLevel(GWorld, "L_Battle_Arena");
}

