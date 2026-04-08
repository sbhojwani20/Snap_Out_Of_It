// Fill out your copyright notice in the Description page of Project Settings.


#include "Tobias/Combat/UI/EnemyWidget.h"
#include "Components/ProgressBar.h"

void UEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnemyWidget::SetHealthBarPercent(const float& a_health)
{
	HealthBar->SetPercent(a_health);
}
