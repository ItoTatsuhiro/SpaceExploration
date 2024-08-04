// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGroup/LevelInterface.h"
#include "Kismet/GameplayStatics.h"
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

	if (BeforeLevel != nullptr) {
		UKismetSystemLibrary::PrintString(this, "BeforeLevel unload", true, true, FColor::Cyan, 2.f, TEXT("None"));
		//レベル消去
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, BeforeLevel, LatentInfo, true);
	}
}

void ALevelInterface::LoadLevel(TSoftObjectPtr<UWorld> nextlevel)
{
	BeforeLevel = NowLevel;
	//新しく現在のレベル設定
	NowLevel = nextlevel;

	UKismetSystemLibrary::PrintString(this, "NowLevel load", true, true, FColor::Cyan, 2.f, TEXT("None"));
	//次のレベルを読み込み
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NowLevel, true, true, LatentInfo);
}

void ALevelInterface::UnLoadLevel()
{
	//現在のレベルを消去
	
}
