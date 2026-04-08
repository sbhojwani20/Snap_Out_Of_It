// Fill out your copyright notice in the Description page of Project Settings.


#include "Emil/GameManager.h"
#include "Kismet/GameplayStatics.h"

// Code from Marcus
//-----------------------------------------------
auto UGameManager::GetManagerInstance(const UObject& a_target) -> UGameManager* const
{
	auto gameInstance = GetGameInstance(a_target);

	if (gameInstance) return gameInstance->GetSubsystem<UGameManager>();

	return nullptr;
}

auto UGameManager::GetGameInstance(const UObject& a_target) -> UGameInstance* const
{
	if (a_target.GetWorld() && a_target.GetWorld()->GetGameInstance())
	{
		return UGameplayStatics::GetGameInstance(a_target.GetWorld());
	}
	else if (a_target.IsA(AActor::StaticClass()))
	{
		if (Cast<AActor>(&a_target)->GetGameInstance()) return GEngine->GetWorld()->GetGameInstance();
	}

	return nullptr;
}

//-----------------------------------------------

void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Display, TEXT("Game Manager initialized!"));


}

void UGameManager::Deinitialize()
{
	UE_LOG(LogTemp, Display, TEXT("Game Manager finalized!"));
}
