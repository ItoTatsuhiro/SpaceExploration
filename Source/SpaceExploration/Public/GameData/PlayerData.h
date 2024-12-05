// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerData.generated.h"

struct FStatus;


USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	FStatus* Status;
};