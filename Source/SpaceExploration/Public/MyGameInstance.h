// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "tsutsumi/Status.h"
#include "tsutsumi/Element.h"
#include "MyGameInstance.generated.h"

UCLASS()
class SPACEEXPLORATION_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	//バトルで使用する敵
	FStatus BattleEnemy;
	//バトルで使用する敵の属性
	EElement BattleEnemyElement = EElement::none;
	//バトルで使用する敵のタイプ

public:
	

	//バトルシーンで使用する敵の情報をセット
	void SetterBattleEnemy(FStatus& battleenemy, const EElement& battleenemyelement);
	//バトルシーンで使用する敵のステータスをゲット
	FStatus GetterBattleEnemyStatus();
	//バトルシーンで使用する敵の属性をゲット
	EElement GetterBattleEnemyElement();
};
