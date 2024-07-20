// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SelectTileScene/AC_MapTileHeal.h"

#include "Scene/SelectTileScene/E_Tile.h"



// コンストラクタ
AAC_MapTileHeal::AAC_MapTileHeal() {


	tileType_ = E_TILE_TYPE::HEAL;

}


// 開始時に実行する関数
void AAC_MapTileHeal::BeginPlay() {



}



// 更新用関数
void AAC_MapTileHeal::Tick(float DeltaTime) {



}


// マスで実行するイベントの関数
// AAC_MapTileBaseクラスのTileEventクラスをオーバーライド
void AAC_MapTileHeal::TileEvent() {

	int32 RecoverHP(100000);

}