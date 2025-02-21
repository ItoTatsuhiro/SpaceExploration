// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/GameData/TileData.h"

UTileData::UTileData()
{
	enemyData_.EnemyElement = EElement::none;
	enemyData_.EnemyType = -1;
	enemyData_.Level = -1;
}

UTileData::~UTileData()
{
}

void UTileData::SetEnemyData(FEnemyData& _enemyData)
{
	enemyData_.EnemyElement = _enemyData.EnemyElement;
	enemyData_.EnemyType = _enemyData.EnemyType;
	enemyData_.Level = _enemyData.Level;
}
