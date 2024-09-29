// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../../tsutsumi/GalaxyRandomSelect.h"
#include "../../Scene/SelectTileScene/E_Tile.h"
#include "../../Manager/SequenceManager.h"


#include "AC_StageMapManager.generated.h"

//// 前方宣言
//class FSequenceDelegate;



// 二重のTArrayに使用する用
// マスのベース型のTArrayをもつ構造体
USTRUCT()
struct FTileArray {
	GENERATED_BODY()

	UPROPERTY()
	TArray<class AAC_MapTileBase*> TileArray;
};


// 作成者：伊藤
// ステージマップを管理するためのクラス
// ステージの配列の生成、保持、取得等の処理を行う
UCLASS(BlueprintType)
class SPACEEXPLORATION_API AAC_StageMapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_StageMapManager();

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//------------------------------------------------------------------------------------
	// MapSceneの制御用
	// デリゲートを用いて制御を行う

	// シーケンスマネージャー
	USequenceManager* sequenceManager_;



	// シーケンス用の関数とデリゲート

	// マス生成シーケンス
	void SeqCreateTile(const float delta_time);
	// マス生成シーケンス用デリゲート(コンストラクタで初期化)
	FSequenceDelegate createTileDel_;



	// マス選択シーケンス
	void SeqSelectTile(const float delta_time);
	// マス選択シーケンス用デリゲート(コンストラクタで初期化)
	FSequenceDelegate selectTileDel_;



	//------------------------------------------------------------------------------------
	// マス生成関連

	// マスランダム生成のためのクラスのインスタンス用
	UPROPERTY()
	AGalaxyRandomSelect* galaxyRandomSelect_ = nullptr;

	// マスをランダム生成するためのGalaxyRandomSelectクラスのコンポーネント
	// ランダムにマスを生成する際に、MakeTileArray関数を呼び出して使用する
	UPROPERTY(VisibleAnywhere)
	class UChildActorComponent* galaxyRandomSelectComponent_;

	// AGalaxyRandomSelectのクラス
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AGalaxyRandomSelect> galaxyRandomSelectClass_;

	UPROPERTY(VisibleAnywhere)
	// マスの種類の配列
	// galaxyRandomSelectで生成した配列を持ってくる
	TArray< FTileEnumArray > tileTypeArray_;

	// 生成したマスのオブジェクトを保持するためのコンポーネント
	UPROPERTY(VisibleAnywhere)
	class UChildActorComponent* tileObjectComponent_;

	// マスのオブジェクトの配列
	// tileTypeArrayから実際にオブジェクトを生成
	UPROPERTY()
	TArray< FTileArray > tileObjArray_;


	// マス同士の間隔
	UPROPERTY(EditAnywhere, Category = "tileDetail")
	float tileSpace_;

	// マスを配置する際の基準となる座標
	UPROPERTY(EditAnywhere, Category = "tileDetail")
	FVector basePos_;


	// バトルマスのクラス
	UPROPERTY(EditDefaultsOnly, Category = "tileDetail")
	TSubclassOf<class AAC_MapTileBattle> battleTileClass_;

	// 回復マスのクラス
	UPROPERTY(EditDefaultsOnly, Category = "tileDetail")
	TSubclassOf<class AAC_MapTileHeal> healTileClass_;

	// アイテムマスのクラス
	UPROPERTY(EditDefaultsOnly, Category = "tileDetail")
	TSubclassOf<class AAC_MapTileItem> itemTileClass_;

	const APlayerCharacter* PlayerCharacter_;


	// レイを飛ばして当たったActorを取得する関数
	AActor* PerformRaycast();

	// カーソルが重なっているマス
	AActor* hoveredTile_;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// マスを追加で生成する関数
	// 引数：createTileNumArray...新しく生成するマスの配列の大きさ
	// ----------------------------------------------------------------------
	// 例）{ 1, 2, 3, 2 }とした場合、以下のようなマスを生成することを想定
	// 3	　〇　〇	
	// 2	〇　〇　〇
	// 1	　〇　〇
	// 0	　　〇
	// ----------------------------------------------------------------------
	void CreateTileObjArray( TArray<int> createTileNumArray );

};
