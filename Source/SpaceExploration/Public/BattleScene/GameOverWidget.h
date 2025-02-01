// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct()override;
	bool Initialize()override;

private:
	AActor* battlemanager;

public:
	//セッター
	void SetBattleManager(AActor* _battlemanager) {
		battlemanager = _battlemanager;
	}

};
