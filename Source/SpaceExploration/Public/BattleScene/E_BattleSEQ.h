// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "E_BattleSEQ.generated.h"

//BattleSceneÇÃçsìÆèá
UENUM(BlueprintType)
enum class E_BattleSEQ : uint8
{
	BATTLE_STANDBY,
	PLAYER_ATTACK,
	PLAYER_ATTACKRECEIVE,
	ENEMY_ATTACK,
	ENEMY_ATTACKRECEIVE,
	BATTLE_RESULT,
	BATTLE_END,
	CAMERA_CHANGE
};
