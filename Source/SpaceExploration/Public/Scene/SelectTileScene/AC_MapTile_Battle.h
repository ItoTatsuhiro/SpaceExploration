// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Scene/SelectTileScene/AC_MapTile.h"
#include "LevelGroup/LevelInterface.h"
#include "../../GameData/EnemyData.h"
#include "AC_MapTile_Battle.generated.h"

class APlaySceneGameModeBase;
class AEnemyBase;

// バトルを行うマスのクラス
// バトル処理は後々
UCLASS(BlueprintType)
class SPACEEXPLORATION_API AAC_MapTileBattle final: public AAC_MapTileBase
{
	GENERATED_BODY()
	
public :

	// コンストラクタ
	AAC_MapTileBattle();


private :

	// 開始時に実行する関数
	void BeginPlay() override;

	//バトルレベルのPath
	const FString BPPath_BattleLevel = "/Game/graphics/alpha/Master_Lv_battlemap/Lv_battle_map";

	// 敵の情報
	UPROPERTY(VisibleAnywhere)
	FEnemyData EnemyData;

public :
	
	// 更新用関数
	void Tick(float DeltaTime) override;

	// マスで実行するイベントの関数
	// AAC_MapTileBaseクラスのTileEventクラスをオーバーライド
	void TileEvent() override;

	// エネミーデータ（情報）を取得する
	UFUNCTION(BlueprintCallable)
	FEnemyData GetEnemyData() { return EnemyData; }

	// エネミーデータ（情報）を設定する
	void SetEnemyData(const FEnemyData& _EnemyData);

};
