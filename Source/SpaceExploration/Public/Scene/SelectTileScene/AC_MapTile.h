// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "E_Tile.h"

#include "AC_MapTile.generated.h"

// マップ上のマスのベースクラス
// 抽象クラス、それぞれのマスのクラスに継承して用いる
UCLASS( Abstract )
class SPACEEXPLORATION_API AAC_MapTileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_MapTileBase();

	// マスの種類のゲッター
	inline E_TILE_TYPE getTileType() { return tileType_; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// マスで実行するイベントの関数
	// 継承先でこの関数をオーバーライドして処理を作成する
	virtual void TileEvent() PURE_VIRTUAL(AAC_MapTileBase::TileEvent, );


	// マスの種類の変数
	E_TILE_TYPE tileType_ = E_TILE_TYPE::NONE;


public:	


	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
