// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Scene/SelectTileScene/AC_MapTile.h"
#include "../Scene/SelectTileScene/E_Tile.h"
#include "GalaxyRandomSelect.generated.h"


// 二重のTArrayに使用する用
// マスの種類のEnum型のTArrayをもつ構造体
USTRUCT()
struct FTileEnumArray {
	GENERATED_BODY()

	UPROPERTY()
	TArray<E_TILE_TYPE> typeArray;
};



// 惑星のマスをランダムに生成する処理を行うクラス
// 堤君が作成したものに伊藤が機能追加
UCLASS(BlueprintType)
class SPACEEXPLORATION_API AGalaxyRandomSelect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGalaxyRandomSelect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// ランダムなマスの種類を返す関数
	// （伊藤一部変更）
	E_TILE_TYPE RandomTileSelect();

	// 配列上のランダムな位置に引数の種類のマスを挿入する関数
	void RandomInsertTileType(E_TILE_TYPE insertTile);



	UPROPERTY(VisibleAnywhere)
	// 生成するマスの配列
	// （伊藤追加）
	TArray<FTileEnumArray> tileArray_;



	// 各列におけるマスの数
	// この数分マスを生成する
	// コンストラクタで初期化
	// （伊藤追加）
	// ----------------------------------------------------------------------
	// 例）{ 1, 2, 3, 2 }とした場合、以下のようなマスを生成することを想定
	// 3	　〇　〇	
	// 2	〇　〇　〇
	// 1	　〇　〇
	// 0	　　〇
	// ----------------------------------------------------------------------
	UPROPERTY(EditAnywhere)
	TArray<int> tileNumHolizontal_;


	// マスの種類の数、コンストラクタで設定
	int tileTypeNum_;

	// マス全体の数
	int tileCount_;

	// マスの生成される比重
	UPROPERTY(EditAnywhere)
	TArray<float> selectWeight_ = { 40.0f, 35.0f, 25.0f };


	// それぞれのマスを最低限生成する数
	// -------------------------------------------------------------------------------
	// 例）マスの種類が3種類の場合、配列の大きさは3、
	//		各種類のマスを最低1つずつ生成したい場合には、{ 1, 1, 1 }を入れる
	// -------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere)
	TArray<int> minSelectNum_ = { 2, 2, 2 };


public:


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// マスの配列を作成する関数
	// この関数を呼び出して配列を作成する
	// 引数		：ランダム生成するステージのマスの数の配列
	// 戻り値	：ランダム生成したマスの種類の配列
	// （伊藤追加）
	TArray<FTileEnumArray> MakeTileArray(TArray<int> tileNumHolizontal);


	// 配列の添え字をずらす関数
	// ずらした先にマスがあるか確認し、マスがある位置までずらす
	FVector2D CheckShiftArray(FVector2D before);


	//----------------------------------------------------------------------------------------
	// ゲッターセッター

	// マスの配列のゲッター
	// （伊藤追加）
	TArray<FTileEnumArray> GetTileArray() const { return tileArray_; }



	////惑星の配列
	//E_TILE_TYPE PlanetLine[3][3];	


private:
	// 必要に応じて、プライベートメンバーを追加してください
};
