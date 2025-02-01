// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../GameData/StageMapData.h"

#include "StageDataManager.generated.h"

UCLASS()
class SPACEEXPLORATION_API AStageDataManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageDataManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// ステージのデータを保存しておく変数
	FStageMapData stageMapData_;





public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// ----------------------------------------------------------------------------------------------
	// ステージのデータの確認・取得を行う関数
	// 
	// 引数：stagaMapData...既に存在するデータを代入するための変数。
	//		参照で渡すことで、データを入れることができるようにする
	// 
	// １．データが存在する場合は引数のstageMapDataにデータを入れたうえでtrue型を返す
	// ２．データが存在しない場合はデータを入れずにfalse型を返す
	bool TryGetStageMapData(FStageMapData& stageMapData);


	


};
