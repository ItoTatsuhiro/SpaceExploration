// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "E_CharacterActState.generated.h"

UENUM(BlueprintType)
enum class ECharacterActState : uint8
{
	Idle,
	Move,
	Attack
};