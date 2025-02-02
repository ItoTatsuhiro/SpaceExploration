// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyStatusData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnemyStatusData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Defence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Exp;

};