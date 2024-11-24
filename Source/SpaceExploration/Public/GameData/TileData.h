// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Scene/SelectTileScene/E_Tile.h"
#include "NiagaraComponent.h"





// ---------------------------------------------------------------------------------------------
// マスのデータを保持しておくためのクラス
// シーン遷移等で一度リセットされる際に必要な情報を一時的に保持しておくために使用する
// これを配列として持つStageMapDataクラスで管理（作成予定）
// ---------------------------------------------------------------------------------------------
UCLASS(BlueprintType)
class SPACEEXPLORATION_API TileData
{
public:

	TileData();
	~TileData();


protected :

	// マスの種類
	UPROPERTY(VisibleAnywhere)
	E_TILE_TYPE tileType_;


	// マスのナイアガラコンポーネント
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> tileNiagaraComp_;


public :

	// ---------------------------------------------------------------------------------------------
	// ゲッターセッター

	// ----------------------------------------------------------------------------------
	// ゲッター

	// マスの種類のゲッター
	E_TILE_TYPE GetTileType() { return tileType_; }

	// マスのナイアガラのゲッター
	TObjectPtr<UNiagaraComponent> GetTileNiagaraComp() { return tileNiagaraComp_; }


	// ----------------------------------------------------------------------------------
	// セッター

	// マスの種類のセッター
	void SetTileType(E_TILE_TYPE tileType) { tileType_ = tileType; }

	// マスのナイアガラのセッター
	void SetTileNiagaraComp(TObjectPtr<UNiagaraComponent> tileNiagaraComp) { tileNiagaraComp_ = tileNiagaraComp; }


};
