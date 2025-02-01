// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/StageDataManager.h"

// Sets default values
AStageDataManager::AStageDataManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageDataManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageDataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// ----------------------------------------------------------------------------------------------
// ステージのデータの確認・取得を行う関数
// 
// 引数：stagaMapData...既に存在するデータを代入するための変数。
//		参照で渡すことで、データを入れることができるようにする
// 
// １．データが存在する場合は引数のstageMapDataにデータを入れたうえでtrue型を返す
// ２．データが存在しない場合はデータを入れずにfalse型を返す
bool AStageDataManager::TryGetStageMapData(FStageMapData& stageMapData)
{

	// マップデータが既に有効なとき
	if (stageMapData_.bIsValid_) {

		// 引数として渡した変数に有効なマップデータを入れる
		stageMapData = stageMapData_;
		
		return true;
	}

	// 有効なマップデータが存在しなかったときfalse
	return false;
}

