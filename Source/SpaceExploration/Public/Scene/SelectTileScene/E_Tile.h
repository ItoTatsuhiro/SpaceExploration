// Fill out your copyright notice in the Description page of Project Settings.

// ****************************************************
// ローグライクのマスに関係するEnum Classのファイル
// ****************************************************

#pragma once

#include "CoreMinimal.h"
#include "E_Tile.generated.h"


// マスの種類に関するEnumClass
UENUM(BlueprintType)
enum class E_TILE_TYPE : uint8
{
	BATTLE,				// バトルマス
	ITEM,				// アイテムマス
	HEAL,				// 回復マス
	TILE_TYPE_NUM,		// マスの種類の数	
	NONE				// 種類未設定の状態
};


