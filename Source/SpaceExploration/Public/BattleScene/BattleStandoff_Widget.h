// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleStandoff_Widget.generated.h"

class ABattleManager;

UCLASS()
class SPACEEXPLORATION_API UBattleStandoff_Widget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct()override;
	bool Initialize()override;

private:
	

public:
	//バトルマネージャー
	UPROPERTY(BlueprintReadOnly)
	AActor* battlemanager;

	//セッター
	void SetBattleManager(AActor* _battlemanager) {
		battlemanager = _battlemanager;
	}
};
