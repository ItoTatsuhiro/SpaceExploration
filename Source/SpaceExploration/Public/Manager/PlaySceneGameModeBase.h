// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlaySceneGameModeBase.generated.h"

class ALevelInterface;

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
	// レベルインターフェースをセットする
	void SetLevelInterface(ALevelInterface* LevelInterface)
	{
		LevelInterfaceRef = LevelInterface;
	}

	// レベル移動
	//引数１：次のレベル
	//引数２：移動前のレベル
	//引数３：移動前のレベルを消去するかフラグ（trueで消去）
	//引数４：移動前のレベルを表示するかのフラグ（falseで非表示、引数３がfalseの場合のみ有効）
	UFUNCTION(BlueprintCallable)
	void ChangeLevel(TSoftObjectPtr<UWorld> NextLevel, TSoftObjectPtr<UWorld> NowLevel = nullptr
		, bool BeforeLevelUnload = true, bool BeforeLevelVisible = false);

};
