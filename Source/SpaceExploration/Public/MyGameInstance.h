// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Character\EnemyBase.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//サブレベル
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> BattleLevel;
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> MapLevel;
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> TreasureLevel;

public:
	//バトルで使用する敵
	AEnemyBase* BattleEnemy = nullptr;
	//バトルで使用する敵の属性
	uint8 BattleEnemyElement = 5;

	//バトルシーンで使用する敵の情報をセット
	void SetterBattleEnemy(AEnemyBase* battleenemy, const uint8& battleenemyelement);
	//バトルシーンで使用する敵のステータスをゲット
	AEnemyBase* GetterBattleEnemyStatus();
	//バトルシーンで使用する敵の属性をゲット
	uint8 GetterBattleEnemyElement();
};
