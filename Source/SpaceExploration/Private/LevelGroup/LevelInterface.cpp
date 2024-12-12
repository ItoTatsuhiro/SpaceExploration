// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGroup/LevelInterface.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/PlaySceneGameModeBase.h"
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

	//GameMode取得
	gamemode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (gamemode) {
		UE_LOG(LogClass, Log, TEXT("gamode yes"));
		gamemode->SetLevelInterface(this);
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("gamoemode no"));
	}

	if (LoadingLevel.IsNull()) {
		UE_LOG(LogClass, Warning, TEXT("error : No LoadingLevel BeginPlay"));
	}

	if (NextLevel.IsNull()) {
		UE_LOG(LogClass, Warning, TEXT("error : No NextLevel BeginPlay"));
	}

	//エディタで設定した初期レベルを読み込み
	Load_LoadingLevel();
}

void ALevelInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//NextLevelに何か入った場合
	if (NextLevel.IsNull()) {

	}
	else {
		UE_LOG(LogClass, Log, TEXT("Yes NextLevel Tick\n"));
		//ローディング画面レベルを読み込んだのち、NextLevel読み込み
		Load_LoadingLevel();
	}
}

void ALevelInterface::ChangeLevel(TSoftObjectPtr<UWorld> nextlevel, TSoftObjectPtr<UWorld> nowlevel, bool BeforeLevelUnload, bool BeforeLevelVisible)
{
	FLatentActionInfo LatentInfo;

	if (nextlevel.IsNull()) {
		UE_LOG(LogClass, Warning, TEXT("NO nextlevel LevelInterface ChangeLevel"));
	}
	else {
		UE_LOG(LogClass, Log, TEXT("YES nextlevel LevelInterface ChangeLevel"));
	}

	//NowLevelの消去
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, NowLevel, LatentInfo, false);
	

	UKismetSystemLibrary::PrintString(this, "~~ChangeLevel~~", true, true, FColor::Cyan, 2.f, TEXT("None"));
	//次のレベル設定
	//NextLevelに何か入るとシーン遷移の処理が始まる
	NextLevel = nextlevel;
}

void ALevelInterface::Load_LoadingLevel()
{
	UE_LOG(LogClass, Log, TEXT("------------------------------------Level Loading----------------------------"));

	//非同期処理よう構造体（処理後の関数を呼ぶよう）
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	//”エディタで設定した初期レベル、または次のレベル”を読み込み関数
	LatentInfo.ExecutionFunction = FName("Load_NextLevel");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;
	//ローディング画面レベル読み込み、表示
	if (LoadingLevel.IsNull()) {
		//UE_LOG(LogClass, Warning, TEXT("error : No LoadingLevel Function"));
	}
	else {
		//UE_LOG(LogClass, Log, TEXT("Yes LoadingLevel Function"));
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, LoadingLevel, true, true, LatentInfo);
		//UKismetSystemLibrary::PrintString(this, "load LoadingLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
	}	
}

void ALevelInterface::testdelay() {
	//非同期処理よう構造体（処理後の関数を呼ぶよう）
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	//ロード画面レベル消去関数
	LatentInfo.ExecutionFunction = FName("Unload_LoadingLevel");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;
	//次のレベル読み込み
	if (NextLevel.IsNull()) {
		//UE_LOG(LogClass, Warning, TEXT("error : No NextLevel Function\n"));
	}
	else {
		//NextLevelが読み込まれてない場合
		
		//UE_LOG(LogClass, Log, TEXT("Yes NextLevel noloaded Function"));
		//読み込み
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NextLevel, true, true, LatentInfo);
		//UKismetSystemLibrary::PrintString(this, "load FirstLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
	}
}

void ALevelInterface::Load_NextLevel()
{
	//テスト用、testdelay()の処理をdelayする
	world = GEngine->GameViewport->GetWorld();
	FTimerHandle timerhandle;
	world->GetTimerManager().SetTimer(timerhandle, this, &ALevelInterface::testdelay, 2.0f, false);
}

void ALevelInterface::Unload_LoadingLevel()
{
	FLatentActionInfo LatentInfo;

	//ローディング画面レベルunload
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, LoadingLevel, LatentInfo, false);
	//UKismetSystemLibrary::PrintString(this, "Unload LoadingLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
	//UE_LOG(LogClass, Log, TEXT("LevelInterface:Unload_Loading:Unload_LoadingLevel"));

	//NextLevelをNowLevelに設定
	NowLevel = NextLevel;
	//NextLevelをクリア
	NextLevel = nullptr;
}
