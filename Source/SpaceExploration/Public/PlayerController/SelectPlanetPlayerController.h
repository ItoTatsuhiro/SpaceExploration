// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SelectPlanetPlayerController.generated.h"

class AAC_StageMapManager;
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
	// StageMapManagerのアドレスをセット
	// =========================================
	void SetStageMapManager(AAC_StageMapManager* StageMapManager) 
	{ 
		StageMapManagerRef = StageMapManager;
	}

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

private:
	// 入力マッピングコンテキスト
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// 左クリック入力アクション
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LeftClickAction;

	// StageMapManagerの参照
	UPROPERTY()
	AAC_StageMapManager* StageMapManagerRef;

private:
	// 左クリック時に実行する関数
	void LeftClickEvent();

};
