

#include "Scene/SelectTileScene/AC_MapTile_Battle.h"

#include "Scene/SelectTileScene/E_Tile.h"



// コンストラクタ
AAC_MapTileBattle::AAC_MapTileBattle() {


	tileType_ = E_TILE_TYPE::BATTLE;

}


// 開始時に実行する関数
void AAC_MapTileBattle::BeginPlay() {
	Super::BeginPlay();


}



// 更新用関数
void AAC_MapTileBattle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


}


// マスで実行するイベントの関数
// AAC_MapTileBaseクラスのTileEventクラスをオーバーライド
void AAC_MapTileBattle::TileEvent() {
	Super::TileEvent();

	
}