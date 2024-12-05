// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SelectPlanetPlayerController.generated.h"

class AAC_SceneManagerBase;
class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API ASelectPlanetPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	// =========================================
	// 現在のSceneManagerの参照をセット
	// =========================================
	void SetStageMapManager(AAC_SceneManagerBase* StageMapManager)
	{ 
		CurrentSceneManagerRef = StageMapManager;
	}

	// 入力アクションのセットアップを行う
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

private:
	// 入力マッピングコンテキスト
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// 左クリック入力アクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LeftClickAction;

	// 現在のSceneManagerの参照
	UPROPERTY()
	AAC_SceneManagerBase* CurrentSceneManagerRef;

private:
	// 左クリック時に実行する関数
	void LeftClickEvent();

};
