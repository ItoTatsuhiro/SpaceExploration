// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlaySceneGameModeBase.generated.h"

class ALevelInterface;
class UHealingDisplay;

struct FStageMapData;

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API APlaySceneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	ALevelInterface* LevelInterfaceRef;

public:
	APlaySceneGameModeBase();

	void BeginPlay() override;

	// レベルインターフェースをセットする
	void SetLevelInterface(ALevelInterface* LevelInterface)
	{
		LevelInterfaceRef = LevelInterface;
	}

	// --------------------------------------------------------------------------------------------
	// レベル移動
	//引数１：次のレベル
	//引数２：移動前のレベル
	//引数３：移動前のレベルを消去するかフラグ（trueで消去）
	//引数４：移動前のレベルを表示するかのフラグ（falseで非表示、引数３がfalseの場合のみ有効）
	// --------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void ChangeLevel(TSoftObjectPtr<UWorld> NextLevel, TSoftObjectPtr<UWorld> NowLevel = nullptr
		, bool BeforeLevelUnload = true, bool BeforeLevelVisible = false);

	// ================================================================================
	// ↓↓↓ ステージマップ関連の関数 ↓↓↓
	// ================================================================================

	// --------------------------------------------------------------------------------------------
	// ステージマップデータをセットする
	// --------------------------------------------------------------------------------------------
	void SetStageMapData(FStageMapData* _StageMapData)
	{
		StageMapData = _StageMapData;
	}

	// --------------------------------------------------------------------------------------------
	// ステージマップデータの取得を試す
	// 
	// 引数１...取得させるStageMapData型
	// 
	// 戻り値...true：成功 false：失敗
	// --------------------------------------------------------------------------------------------
	bool TryGetStageMapData(FStageMapData* _StageMapData);

	// --------------------------------------------------------------------------------------------
	// 回復画面のウィジェットを返す
	// --------------------------------------------------------------------------------------------
	UHealingDisplay* GetHealingDisplayWhidget();

private:
	// 現在のステージの各惑星データの持つ構造体
	FStageMapData* StageMapData;

	// 回復画面のUserWidgetクラス
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealingDisplay> HealingDisplayClass;

	// 回復画面のUserWidget
	UPROPERTY()
	UHealingDisplay* HealingDisplayWidget;

};
