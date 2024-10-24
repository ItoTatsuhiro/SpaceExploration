// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGroup/LevelInterface.h"
#include "Character/CharacterBase.h"
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

	if (NextLevel.IsNull()) {
		UE_LOG(LogClass, Warning, TEXT("error : No NextLevel\n"));
		NextLevel = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("Game/mapScene/MapScene")));
	}
	if (LoadingLevel.IsNull()) {
		UE_LOG(LogClass, Warning, TEXT("error : No LoadingLevel\n"));
		LoadingLevel = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("Game/LevelGroup/L_LoadingLevel")));
	}

	//エディタで設定した初期レベルを読み込み
	Load_LoadingLevel();
}

// Called every frame
void ALevelInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//NextLevelに何か入った場合
	if (NextLevel.IsValid()) {
		//ローディング画面レベルを読み込んだのち、NextLevel読み込み
		Load_LoadingLevel();
	}
	else {

	}
}

void ALevelInterface::ChangeLevel(TSoftObjectPtr<UWorld> nextlevel)
{
	//次のレベル設定
	NextLevel = nextlevel;
}

void ALevelInterface::Load_LoadingLevel()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	//”エディタで設定した初期レベル、または次のレベル”を読み込み関数
	LatentInfo.ExecutionFunction = FName("Load_NextLevel");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;
	//ローディング画面レベル読み込み、表示
	if (LoadingLevel.IsValid()) {
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, LoadingLevel, true, false, LatentInfo);
		UKismetSystemLibrary::PrintString(this, "load LoadingLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("error : No LoadingLevel\n"));
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NextLevel, true, false, LatentInfo);
	}	
}

void ALevelInterface::testdelay() {
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	//ロード画面レベル消去関数
	LatentInfo.ExecutionFunction = FName("Unload_LoadingLevel");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;
	//初期ワールド読み込み
	if (NextLevel.IsValid()) {
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NextLevel, true, true, LatentInfo);
		UKismetSystemLibrary::PrintString(this, "load FirstLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("error : No NextLevel\n"));
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NowLevel, true, false, LatentInfo);
	}
}

void ALevelInterface::Load_NextLevel()
{
	world = GEngine->GameViewport->GetWorld();
	FTimerHandle timerhandle;
	world->GetTimerManager().SetTimer(timerhandle, this, &ALevelInterface::testdelay,1.0f, false);
}

void ALevelInterface::Unload_LoadingLevel()
{
	FLatentActionInfo LatentInfo;
	
	//ローディング画面レベルunload
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, LoadingLevel, LatentInfo, false);
	UKismetSystemLibrary::PrintString(this, "Unload LoadingLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));

	//NowLevelの中身があればunload
	if (NowLevel.IsValid()) {
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, NowLevel, LatentInfo, false);
		UKismetSystemLibrary::PrintString(this, "Unload NowLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
	}
	
	//NextLevelをNowLevelに設定
	NowLevel.Reset();
	NowLevel = NextLevel;
	//NextLevelをクリア
	NextLevel.Reset();
}
