// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/EnemySpawn.h"
#include "Tobias/Combat/CombatManager.h"
#include "Shayna/Enemy/EnemyAI.h"
#include "Tobias/Combat/UI/EnemyWidget.h"

// Sets default values
AEnemySpawn::AEnemySpawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp")));
}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();

	auto manager = UCombatManager::GetManagerInstance(*this);

	manager->SetEnemyStart(this);

	Enemy = Cast<AEnemyAI>(GetWorld()->SpawnActor(AEnemyAI::StaticClass(), &GetActorTransform(), FActorSpawnParameters()));
	Enemy->SetType(manager->GetEncounter());
	Enemy->SetUpEntity();

	manager->SetEnemy(Cast<ACombatEntity>(Enemy));

	if (Enemy) Enemy->GetMesh()->PlayAnimation(Enemy->GetStats().IdleCombatAnimSequ.Get(), true);

	GetWorldTimerManager().SetTimer(HandleStartOfCombat, manager, &UCombatManager::StartCombat, 1.f, false);
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
