// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SelectTileScene/AC_MapTileItem.h"

#include "Scene/SelectTileScene/E_Tile.h"


// コンストラクタ
AAC_MapTileItem::AAC_MapTileItem() {


	tileType_ = E_TILE_TYPE::ITEM;

}


// 開始時に実行する関数
void AAC_MapTileItem::BeginPlay() {

	////移動先のレベルをパスで取得
	//FSoftObjectPath nextlevelpath(TEXT(""));
	////パスで取得したレベルを設定
	//NextLevel = TSoftObjectPtr<UWorld>(nextlevelpath);
}



// 更新用関数
void AAC_MapTileItem::Tick(float DeltaTime) {



}


// マスで実行するイベントの関数
// AAC_MapTileBaseクラスのTileEventクラスをオーバーライド
void AAC_MapTileItem::TileEvent() {
	//宝箱シーンに移動
	//gamemode->ChangeLevel(NextLevel, this, false, false);

}