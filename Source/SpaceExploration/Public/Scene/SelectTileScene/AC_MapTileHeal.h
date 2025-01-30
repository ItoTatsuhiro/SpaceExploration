// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scene/SelectTileScene/AC_MapTile.h"
#include "AC_MapTileHeal.generated.h"

class UHealingDisplay;
class APlayerCharacter;

// 回復を行うマスのクラス
// 回復処理は後々
UCLASS(BlueprintType)
class SPACEEXPLORATION_API AAC_MapTileHeal : public AAC_MapTileBase
{
	GENERATED_BODY()
	

public:

	// コンストラクタ
	AAC_MapTileHeal();


private:

	// 開始時に実行する関数
	void BeginPlay() override;


public:


	// 更新用関数
	void Tick(float DeltaTime) override;


	// マスで実行するイベントの関数
	// AAC_MapTileBaseクラスのTileEventクラスをオーバーライド
	void TileEvent() override;

	void TileEventRunning() override;

private:
	// プレイヤーの参照
	UPROPERTY()
	APlayerCharacter* PlayerCharacterRef;

	// 回復画面のUserWidgetクラス
	UPROPERTY()
	UHealingDisplay* HealingDisplayWidgetRef;

};
