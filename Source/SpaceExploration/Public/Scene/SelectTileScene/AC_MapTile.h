// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "E_Tile.h"
#include "../../Character/MouseButtonEvent.h"
#include "AC_MapTile.generated.h"

class APlaySceneGameModeBase;
class UNiagaraComponent;

//********************************************************************
// マップ上のマスのベースクラス
// 抽象クラス、それぞれのマスのクラスに継承して用いる
UCLASS( Abstract )
class SPACEEXPLORATION_API AAC_MapTileBase : public AActor, public IMouseButtonEvent
{
	GENERATED_BODY()
	
public:	
	// コンストラクタ
	AAC_MapTileBase();

	// マスの種類のゲッター
	inline E_TILE_TYPE getTileType() { return tileType_; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// マスで実行するイベントが終了しているかどうか
	// デフォルト値はfalse
	UPROPERTY(EditAnywhere)
	bool isTileEventEnd;

	// マスの種類の変数
	// デフォルト値はNONE
	UPROPERTY(EditAnywhere)
	E_TILE_TYPE tileType_;


	//------------------------------------------------------------------
	// メッシュ関連

	// SceneComponent
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> defaultSceneRoot_;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> staticMesh_;

	// 惑星のナイアガラコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> planetNiagaraComp_;


	//シーン遷移ようGamemode
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlaySceneGameModeBase> gamemode;

	//遷移先のレベル
	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSoftObjectPtr<UWorld> NextLevel = nullptr;

	UPROPERTY()
	bool IsEvnetCompleted;

public:	


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// マスで実行するイベントの関数
	// 継承先でこの関数をオーバーライドして処理を作成する
	virtual void TileEvent() PURE_VIRTUAL(AAC_MapTileBase::TileEvent, );

	// マスのイベント実行中の処理関数
	virtual void TileEventRunning() PURE_VIRTUAL(AAC_MapTileBase::TileEventRunning, );

	// スタティックメッシュをセットする関数
	// 引数：fileName...スタティックメッシュの保存先のパス
	// 
	// 使用例）SetStaticMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	void SetStaticMesh(const TCHAR* fileName);

	// 左クリックをされた時の処理を行う。
	// プレイヤーを自身の惑星まで移動させる。
	// 
	// 引数：PlayerCharacter...プレイヤーのリファレンス
	virtual void LeftMouseButtonEvent_Implementation(APlayerCharacter* PlayerCharacter) override;

};
