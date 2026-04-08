// Fill out your copyright notice in the Description page of Project Settings.


#include "Shayna/World/LevelTransitionManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../SnapOutOfItCharacter.h"


// Sets default values
ALevelTransitionManager::ALevelTransitionManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = TransitionVolume;
	
	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionManager::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ALevelTransitionManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelTransitionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelTransitionManager::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASnapOutOfItCharacter* PlayerCharacter = Cast<ASnapOutOfItCharacter>(OtherActor);

	if (PlayerCharacter) ChangeLevel();
}

void ALevelTransitionManager::ChangeLevel()
{
	if (!NextLevelName.IsNone()) UGameplayStatics::OpenLevel(this, NextLevelName);
}

