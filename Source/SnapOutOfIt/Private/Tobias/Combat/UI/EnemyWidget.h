// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetHealthBarPercent(const float& a_health);
	
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;
};
