// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"


void UMyGameInstance::SetterBattleEnemy(FStatus& battleenemy, const EElement& battleenemyelement)
{
	BattleEnemy = battleenemy;
	BattleEnemyElement = battleenemyelement;
}

FStatus UMyGameInstance::GetterBattleEnemyStatus()
{
	return BattleEnemy;
}

EElement UMyGameInstance::GetterBattleEnemyElement()
{
	return BattleEnemyElement;
}
