// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString EnemyName = FString(TEXT(""));

	UPROPERTY(EditAnywhere)
	bool bEnemyAlive = true;
};

// enum + name combatmanager �bergeben
// wenn enemy stirbt bool false
// wenn level l�dt alle datens�tze �berpr�fen
// die die tot sind destroyen
// 
// enemy constructor: datatable holen
// if row exists with enemy name
// if bool false then destroy enemy and delete row
// if bool true then continue
// 
// else row doesn't exist with enemy name
// default struct bool true