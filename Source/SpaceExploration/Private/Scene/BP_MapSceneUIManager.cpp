// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/BP_MapSceneUIManager.h"
#include "LevelGroup/MapSceneUIWidget.h"
#include "Scene/HelpWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Manager/PlaySceneGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABP_MapSceneUIManager::ABP_MapSceneUIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABP_MapSceneUIManager::BeginPlay()
{
	Super::BeginPlay();

	gamemode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(this));
	gamemode->GetStageDataManager()->TryGetStageMapData(_nowstagedata);

	//マップシーンUI生成表示
	MapSceneUIWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*MapSceneWidgetPath)).LoadSynchronous();
	if (MapSceneUIWidgetClass) {
		//マップシーンUI生成
		mapsceneuiwidget = UWidgetBlueprintLibrary::Create(GetWorld(), MapSceneUIWidgetClass, UGameplayStatics::GetPlayerController(this, 0));
		//マップシーンUIをAddViewportに追加
		mapsceneuiwidget->AddToViewport(0);
		
		UMapSceneUIWidget* mapsceneuiwidgetref = Cast<UMapSceneUIWidget>(mapsceneuiwidget);
		if (mapsceneuiwidgetref) {
			mapsceneuiwidgetref->SetMapSceneUIManager(this);
		}
	}

	//ヘルプUI生成
	HelpWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*HelpWidgetPath)).LoadSynchronous();
	if (HelpWidgetClass) {
		//ヘルプUI生成
		helpwidget = UWidgetBlueprintLibrary::Create(GetWorld(), HelpWidgetClass, UGameplayStatics::GetPlayerController(this, 0));
		//ヘルプUIをAddViewportに追加
		helpwidget->AddToViewport(0);


		UHelpWidget* helpwidgetref = Cast<UHelpWidget>(helpwidget);
		if (helpwidgetref) {
			helpwidgetref->SetMapSceneUIManaget(this);
		}
	}

	//ゲームクリアのwidgetblueprintを取得
	FString GameClearWidgetPath = TEXT("/Game/LevelGroup/WBP_GameClear.WBP_GameClear_C");
	GameClearWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*GameClearWidgetPath)).LoadSynchronous();
	if (GameClearWidgetClass) {
		gameclearwidget = UWidgetBlueprintLibrary::Create(GetWorld(), GameClearWidgetClass, UGameplayStatics::GetPlayerController(this, 0));

		gameclearwidget->AddToViewport(0);

	}

	//クリア状態であればゲームクリアwidgetを表示
	if (_nowstagedata.isStageClear_) {
		gameclearwidget->SetVisibility(ESlateVisibility::Visible);
		helpwidget->SetVisibility(ESlateVisibility::Hidden);
		mapsceneuiwidget->SetVisibility(ESlateVisibility::Hidden);
	}
	//それ以外はマップシーンUIを表示
	else {
		gameclearwidget->SetVisibility(ESlateVisibility::Hidden);
		helpwidget->SetVisibility(ESlateVisibility::Hidden);
		mapsceneuiwidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (_nowstagedata.isStageClear_) {
		UE_LOG(LogClass, Warning, TEXT("ABP_MapSceneUIManager::BeginPlay : isClear : TRUE \n"));
	}
	else if (!_nowstagedata.isStageClear_) {
		UE_LOG(LogClass, Warning, TEXT("ABP_MapSceneUIManager::BeginPlay : isClear : FALSE \n"));
	}
	
	UE_LOG(LogClass, Warning, TEXT("~ABP_MapSceneUIManager::BeginPlay~\n"));
}

void ABP_MapSceneUIManager::EndPlay(const EEndPlayReason::Type _endplay)
{
	Super::EndPlay(_endplay);

	mapsceneuiwidget->RemoveFromParent();
	helpwidget->RemoveFromParent();
	gameclearwidget->RemoveFromParent();
}

// Called every frame
void ABP_MapSceneUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isclear = gamemode->GetStageDataManager()->GetIsStageClear();

	if (isclear) {
		gameclearwidget->SetVisibility(ESlateVisibility::Visible);
		helpwidget->SetVisibility(ESlateVisibility::Hidden);
		mapsceneuiwidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

