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

	//引数BeforeLevelUnloadがtrueの場合、移動前のレベルを消去する
	//if (BeforeLevelUnload == true) {
	//	//移動前のレベルを消去
	//	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, nowlevel, LatentInfo, false);
	//	UE_LOG(LogClass, Log, TEXT("levelinterface: changelevel: unload"));
	//}
	//else {
	//	//移動前のレベルを消去しない場合、移動前レベルを表示か非表示か
	//	
	//	////引数BeforeLevelVisibleがfalseの場合非表示
	//	//if (BeforeLevelVisible == false) {
	//	//	//ULevelStreaming*型の変数にTSoftObjectPtr<UWorld>型のワールドを入れられるようにする
	//	//	ULevelStreaming* beforelevel = GetWorld()->GetStreamingLevels()[0];
	//	//	//beforelevelにnowlevelをセット
	//	//	beforelevel->SetWorldAsset(nowlevel);
	//	//	//非表示にする
	//	//	beforelevel->SetShouldBeVisible(false);
	//	//	UE_LOG(LogClass, Log, TEXT("changelevel: novisible"));

	//	//}
	//	//else {
	//	//	UE_LOG(LogClass, Log, TEXT("levelinterface: changelevel: yesvisible"));
	//	//}

	//	//for (ULevelStreaming* streaminglevel : GetWorld()->GetStreamingLevels()) {
	//	//	/*if (streaminglevel && streaminglevel->GetWorldAsset().ToString() == nowlevel.ToString()) {*/

	//	//	if (!streaminglevel) {
	//	//		UE_LOG(LogClass, Log, TEXT("levelinterface: changelevel: streamingLevel is nullptr"));
	//	//		continue;
	//	//	}

	//	//	if (streaminglevel->GetWorldAsset().ToString() != nowlevel.ToString()) {
	//	//		continue;
	//	//	}

	//	//	//更新を止める
	//	//	streaminglevel->SetShouldBeLoaded(false);
	//	//	UE_LOG(LogClass, Log, TEXT("levelinterface: changelevel: update false"));

	//	//	if (!BeforeLevelVisible) {
	//	//		//非表示にする
	//	//		streaminglevel->SetShouldBeVisible(false);
	//	//		UE_LOG(LogClass, Log, TEXT("levelinterface: changelevel: visible false"));
	//	//	}
	//	//}

	//	ULevelStreaming* Level = GetWorld()->GetStreamingLevels()[0];
	//	Level->SetWorldAsset(nowlevel);
	//	Level->SetShouldBeLoaded(false);
	//	UE_LOG(LogClass, Log, TEXT("levelinterface: changelevel: update false"));
	//	if (!BeforeLevelVisible) {
	//		Level->SetShouldBeVisible(false);
	//		UE_LOG(LogClass, Log, TEXT("levelinterface: changelevel: visible false"));
	//	}
	//}

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
		UE_LOG(LogClass, Warning, TEXT("error : No LoadingLevel Function"));
	}
	else {
		UE_LOG(LogClass, Log, TEXT("Yes LoadingLevel Function"));
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, LoadingLevel, true, true, LatentInfo);
		UKismetSystemLibrary::PrintString(this, "load LoadingLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
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
		UE_LOG(LogClass, Warning, TEXT("error : No NextLevel Function\n"));
	}
	else {
		//ULevelStreaming* nextlevel = GetWorld()->GetStreamingLevels()[0];
		//nextlevel->SetWorldAsset(NextLevel);
		////NextLevelが読み込み済みの場合
		//if (nextlevel->IsLevelLoaded()) {
		//	UE_LOG(LogClass, Log, TEXT("Yes NextLevel yesloaded Function"));
		//	//非表示の場合、表示にする
		//	if (!nextlevel->GetShouldBeVisibleFlag()) {
		//		nextlevel->SetShouldBeVisible(true);
		//	}

		//	//ローディング画面をUnload
		//	Unload_LoadingLevel();
		//}
		////読み込んでない場合は通常通りレベル読み込み
		//else {
		//	UE_LOG(LogClass, Log, TEXT("Yes NextLevel noloaded Function"));
		//	//読み込み
		//	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NextLevel, true, true, LatentInfo);
		//	UKismetSystemLibrary::PrintString(this, "load FirstLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
		//}

		//ULevelStreaming* nextlevel = GetWorld()->GetStreamingLevels()[0];
		//nextlevel->SetWorldAsset(NextLevel);
		////NextLevelが読み込み済みの場合
		//if (nextlevel->IsLevelLoaded()) {
		//	//for (ULevelStreaming* streaminglevel : GetWorld()->GetStreamingLevels()) {
		//		////***NextLevelが同じでない可能性あり***
		//		//if (streaminglevel && streaminglevel->GetWorldAsset().ToString() == NextLevel.ToString()) {
		//		//	//更新を再開
		//		//	streaminglevel->SetShouldBeLoaded(true);
		//		//	UE_LOG(LogClass, Log, TEXT("levelinterface: testdelay: update true"));

		//		//	//非表示の場合表示にする
		//		//	streaminglevel->SetShouldBeVisible(true);
		//		//	UE_LOG(LogClass, Log, TEXT("levelinterface: testdelay: visible true"));

		//		//	//ローディング画面をUnload
		//		//	UKismetSystemLibrary::Delay(this,0.0f, LatentInfo);
		//		//	break;
		//		//}

		//		nextlevel->SetShouldBeLoaded(true);
		//		UE_LOG(LogClass, Log, TEXT("levelinterface: testdelay: update true"));
		//		nextlevel->SetShouldBeVisible(true);
		//		UE_LOG(LogClass, Log, TEXT("levelinterface: testdelay: visible true"));

		//		//ローディング画面をUnload
		//		UKismetSystemLibrary::Delay(this,0.0f, LatentInfo);
		//		//break;
		//	//}
		//}
		//NextLevelが読み込まれてない場合
		
		UE_LOG(LogClass, Log, TEXT("Yes NextLevel noloaded Function"));
		//読み込み
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NextLevel, true, true, LatentInfo);
		UKismetSystemLibrary::PrintString(this, "load FirstLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
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
	UKismetSystemLibrary::PrintString(this, "Unload LoadingLevel", true, true, FColor::Cyan, 5.f, TEXT("None"));
	UE_LOG(LogClass, Log, TEXT("LevelInterface:Unload_Loading:Unload_LoadingLevel"));

	//NextLevelをNowLevelに設定
	NowLevel = NextLevel;
	//NextLevelをクリア
	NextLevel = nullptr;
}
