// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "LevelGroup/LevelInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ALevelInterface::ALevelInterface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelInterface::BeginPlay()
{
	Super::BeginPlay();
	
	
	//初期ワールドほ読み込み
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, FirstLevel, true, true, LatentInfo);

	//NowLevelを設定
	NowLevel = FirstLevel;
	
}

// Called every frame
void ALevelInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//BeforeLevelに中身があればUnloadし消去する
	if (BeforeLevel != nullptr) {
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, BeforeLevel, LatentInfo, true);

		//BeforeLevel消去（開放）
		BeforeLevel.Reset();
		BeforeLevel = nullptr;
	}
}

void ALevelInterface::LoadLevel(TSoftObjectPtr<UWorld> nextlevel)
{
	//現在のレベルを前のレベルにする
	BeforeLevel = NowLevel;
	//新しく現在のレベル設定
	NowLevel = nextlevel;

	//次のレベルを読み込み
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NowLevel, true, true, LatentInfo);
}

void ALevelInterface::UnLoadLevel()
{
	//現在のレベルを消去
	
}

void ALevelInterface::testfanc(int aaa)
{
	UKismetSystemLibrary::PrintString(this, "!!!test fanc printstring!!!", true, true, FColor::Cyan, 2.f, TEXT("None"));
}
