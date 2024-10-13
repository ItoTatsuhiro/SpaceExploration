// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"


void UMyGameInstance::SetterBattleEnemy(AEnemyBase* battleenemy, const uint8& battleenemyelement)
{
	BattleEnemy = battleenemy;
	BattleEnemyElement = battleenemyelement;
}

AEnemyBase* UMyGameInstance::GetterBattleEnemyStatus()
{
	return BattleEnemy;
}

uint8 UMyGameInstance::GetterBattleEnemyElement()
{
	return BattleEnemyElement;
}
