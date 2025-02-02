// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SelectTileScene/AC_MapTileHeal.h"

#include "Scene/SelectTileScene/E_Tile.h"

#include "Kismet/GameplayStatics.h"
#include "Manager/PlaySceneGameModeBase.h"
#include "Character/PlayerCharacter.h"
#include "UI/HealingDisplay.h"



// コンストラクタ
AAC_MapTileHeal::AAC_MapTileHeal() {

	HealingDisplayWidgetRef = nullptr;

	tileType_ = E_TILE_TYPE::HEAL;

}


// 開始時に実行する関数
void AAC_MapTileHeal::BeginPlay()
{
	Super::BeginPlay();

	APlaySceneGameModeBase* PlaySceneGameMode = Cast<APlaySceneGameModeBase>( UGameplayStatics::GetGameMode( GetWorld() ) );

	if (!PlaySceneGameMode)
	{
		UE_LOG(LogClass, Error, TEXT("AAC_TileHeal::BeginPlay() : error PlaySceneGameMode が nullptr でした"));
		return;
	}

	HealingDisplayWidgetRef = PlaySceneGameMode->GetHealingDisplayWhidget();
	PlayerCharacterRef = Cast<APlayerCharacter>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	if (!PlayerCharacterRef)
	{
		UE_LOG(LogClass, Error, TEXT("AAC_TileHeal::BeginPlay() : error PlayerCharacterRef の取得に失敗しました。"));
	}

}



// 更新用関数
void AAC_MapTileHeal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// マスで実行するイベントの関数
// AAC_MapTileBaseクラスのTileEventクラスをオーバーライド
void AAC_MapTileHeal::TileEvent() {

	if (!PlayerCharacterRef)
	{
		UE_LOG(LogClass, Error, TEXT("AAC_TileHeal::TileEvent() : error PlayerCharacterRef が nullptr でした"));
		return;
	}

	if (!HealingDisplayWidgetRef)
	{
		PlayerCharacterRef->RecoverHP();
		UE_LOG(LogClass, Error, TEXT("AAC_TileHeal::TileEvent() : error HealingDisplayWidgetRef が nullptr でした"));
		return;
	}

	// 回復画面を表示
	HealingDisplayWidgetRef->SetHpStatus(PlayerCharacterRef->GetCharacterStatus());
	HealingDisplayWidgetRef->AddToViewport(0);

	PlayerCharacterRef->RecoverHP();

	bIsEvnetCompleted = false;

}

void AAC_MapTileHeal::TileEventRunning()
{
	if ( !HealingDisplayWidgetRef->IsProcessCompleted() ) 
	{
		return;
	}

	// 回復画面を消す
	HealingDisplayWidgetRef->RemoveFromParent();
	bIsEvnetCompleted = true;
}
