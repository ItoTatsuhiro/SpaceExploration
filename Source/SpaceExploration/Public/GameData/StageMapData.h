// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Scene/SelectTileScene/AC_StageMapManager.h"
#include "../../Public/GameData/TileData.h"
#include "StageMapData.generated.h"

// --------------------------------------------------------
// 二次元配列として扱うために用いる構造体
// これを配列にすることで二次元配列として扱う
// 
// ※※※※※※※※※※※※※※※※※※※※※※※※※※※
// こちらは別の場所では使用しない！！！
// (FStageMapDataで配列にして使用する用途のため）
// ※※※※※※※※※※※※※※※※※※※※※※※※※※※
// --------------------------------------------------------
USTRUCT()
struct FTileDataArray {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<UTileData*> tileDataArray_;

};


// --------------------------------------------------------
// ステージのデータを保存して置いておくための構造体
// マスデータのクラスの配列を持つ
// 
// この構造体を常に置いておき、
// サブシーンのセーブロード時にマップデータを保持するようにする
// --------------------------------------------------------
USTRUCT (BlueprintType)
struct FStageMapData
{
	GENERATED_BODY()


	// マスのデータの配列
	UPROPERTY(VisibleAnywhere)
	TArray<FTileDataArray> tileDataArray_;

};