// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../GameData/StageMapData.h"
#include "../tsutsumi/GalaxyRandomSelect.h"

#include "StageDataManager.generated.h"

UCLASS(BlueprintType)
class SPACEEXPLORATION_API AStageDataManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageDataManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//------------------------------------------------------------------------------------
	// 現在のステージの情報

	// マスの種類の配列
	// galaxyRandomSelectで生成した配列を持ってくる
	UPROPERTY(VisibleAnywhere)
	TArray< FTileEnumArray > tileTypeArray_;
	

	// ステージのデータを保存しておく変数
	UPROPERTY(VisibleAnywhere)
	FStageMapData stageMapData_;

	// 現在いるステージ上のマスの番号を表す配列
	UPROPERTY(VisibleAnywhere)
	FVector2D nowTileIndex_;


	//------------------------------------------------------------------------------------
	// マス生成関連

	// マスランダム生成のためのクラスのインスタンス用
	UPROPERTY(EditAnywhere)
	AGalaxyRandomSelect* galaxyRandomSelect_ = nullptr;

	// マスをランダム生成するためのGalaxyRandomSelectクラスのコンポーネント
	// ランダムにマスを生成する際に、MakeTileArray関数を呼び出して使用する
	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* galaxyRandomSelectComponent_;


	// マスの情報を保存するUTileDataのクラス
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UTileData> tileDataClass_;

	
	//------------------------------------------------------------------------------------
	// ナイアガラ

	// マスに使用するナイアガラの配列
	// BP化した先で使用するナイアガラを設定する
	UPROPERTY(EditAnywhere)
	TArray< TObjectPtr<UNiagaraSystem> > tileNiagaraArray_;

	// 現在のステージのエネミーレベル
	UPROPERTY(VisibleAnywhere)
	int CurrentStageEnemyLevel;

	// 現在のステージの武器レベル
	UPROPERTY(VisibleAnywhere)
	int CurrentStageWeaponLevel;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	inline int GetCurrentStageWeaponLevel() { return CurrentStageWeaponLevel; }

	// ----------------------------------------------------------------------
	// プレイヤーのいる座標のマスデータの取得を試す
	// 
	// TileData...マスデータを入れるためのポインタ
	// 
	// 戻り値...成功：true、失敗：false
	// ----------------------------------------------------------------------
	bool TryGetCurrentTileDataForPlayerLocation(UTileData* TileData);

	// ----------------------------------------------------------------------------------------------
	// ステージのデータの確認・取得を行う関数
	// 
	// 引数：stagaMapData...既に存在するデータを代入するための変数。
	//		参照で渡すことで、データを入れることができるようにする
	// 
	// １．データが存在する場合は引数のstageMapDataにデータを入れたうえでtrue型を返す
	// ２．データが存在しない場合はデータを入れずにfalse型を返す
	bool TryGetStageMapData(FStageMapData& stageMapData);

	// ----------------------------------------------------------------------------------------------
	// マスを移動する際に呼び出す関数
	// 現在いるマス nowTilePosIndex_ を移動先のマスに変更する
	// 次のマスに移動する際は呼び出すこと！
	void MoveTile(const FVector2D& nextTileIndex);


	// ----------------------------------------------------------------------------------------------
	// 現在いるマスのインデックスを取得する
	FVector2D GetNowTileIndex() { return nowTileIndex_; }


	// ----------------------------------------------------------------------------------------------
	// マスを追加で生成する関数
	// 引数：tileNumArray...新しく生成するマスの配列の大きさ
	// ----------------------------------------------------------------------
	// 例）{ 1, 2, 3, 2 }とした場合、以下のようなマスを生成することを想定
	// 3	　〇　〇	
	// 2	〇　〇　〇
	// 1	　〇　〇
	// 0	　　〇
	// ----------------------------------------------------------------------
	void CreateTileArray(TArray<int> tileNumArray);

	// ----------------------------------------------------------------------
	// 敵の情報をランダムで生成する
	// 
	// y...マスのy座標
	// ----------------------------------------------------------------------
	FEnemyData CreateRandomEnemyData(int y);


	// ----------------------------------------------------------------------------------------------
	// 指定したフォルダ内のナイアガラを読み込んで保存する関数
	// 
	// 引数：FolderPath...ナイアガラの保存フォルダのパス
	void GetNiagaraSystemsFromFolder(const FString& FolderPath);

};
