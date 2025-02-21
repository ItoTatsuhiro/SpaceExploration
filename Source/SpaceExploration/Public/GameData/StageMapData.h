// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "../Scene/SelectTileScene/AC_StageMapManager.h"
#include "../../Public/GameData/TileData.h"
#include "StageMapData.generated.h"

// --------------------------------------------------------
// 二次元配列として扱うために用いる構造体
// これを配列にすることで二次元配列として扱う
// 
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


	// この構造体が既に有効なものであるか確認するための変数
	// デフォルトはfalse, データを入れて使用する際にtrueにする
	bool bIsValid_ = false;


	// ステージをクリアしているか否か
	bool isStageClear_ = false;

	// マスのデータの配列
	// 最初にデータを入れる際にはbIsValidをtrueに切り替えること！
	UPROPERTY(VisibleAnywhere)
	TArray<FTileDataArray> tileDataArray_;

};