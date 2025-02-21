// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Scene/SelectTileScene/E_Tile.h"
#include "NiagaraComponent.h"
#include "../GameData/EnemyData.h"

#include "TileData.generated.h"


// ---------------------------------------------------------------------------------------------
// マスのデータを保持しておくためのクラス
// シーン遷移等で一度リセットされる際に必要な情報を一時的に保持しておくために使用する
// これを配列として持つStageMapDataクラスで管理（作成予定）
// ---------------------------------------------------------------------------------------------
UCLASS()
class SPACEEXPLORATION_API UTileData : public UObject
{
	GENERATED_BODY()

public:

	UTileData();
	~UTileData();


protected :

	// マスの種類
	UPROPERTY(VisibleAnywhere)
	E_TILE_TYPE tileType_;


	// マスのナイアガラコンポーネント
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraSystem> tileNiagaraSys_;
	

	// 配列内でのマスの番号
	UPROPERTY(VisibleAnywhere)
	FVector2D tileArrayIndex_;

	// 敵の基本データ
	UPROPERTY(VisibleAnywhere)
	FEnemyData enemyData_;

	// 移動できるかどうかのフラグ
	UPROPERTY(EditAnywhere)
	bool canMove_ = false;


public :

	// ---------------------------------------------------------------------------------------------
	// ゲッターセッター

	// ----------------------------------------------------------------------------------
	// ゲッター

	// マスの種類のゲッター
	UFUNCTION(BlueprintCallable)
	inline E_TILE_TYPE GetTileType() { return tileType_; }

	// マスのナイアガラのゲッター
	inline TObjectPtr<UNiagaraSystem> GetTileNiagaraSys() { return tileNiagaraSys_; }

	// 配列内のマスの番号のゲッター
	UFUNCTION(BlueprintCallable)
	inline FVector2D GetTileArrayIndex() { return tileArrayIndex_; }


	// canMove_のゲッター
	UFUNCTION(BlueprintCallable)
	inline bool GetCanMove() { return canMove_; }

	// 敵の情報を取得する
	inline FEnemyData GetEnemyData() { return enemyData_; }


	// ----------------------------------------------------------------------------------
	// セッター

	// マスの種類のセッター
	UFUNCTION(BlueprintCallable)
	inline void SetTileType(E_TILE_TYPE tileType) { tileType_ = tileType; }

	// マスのナイアガラのセッター
	inline void SetTileNiagaraSys(TObjectPtr<UNiagaraSystem> tileNiagaraSys) { tileNiagaraSys_ = tileNiagaraSys; }

	// 配列内のマスの番号のセッター
	UFUNCTION(BlueprintCallable)
	inline void SetTileArrayIndex(FVector2D tileArrayIndex) { tileArrayIndex_ = tileArrayIndex; }

	// canMove_のセッター
	UFUNCTION(BlueprintCallable)
	inline void SetCanMove(bool canMove) { canMove_ = canMove; }

	// 敵の情報を設定する
	void SetEnemyData(FEnemyData& _enemyData);

};
