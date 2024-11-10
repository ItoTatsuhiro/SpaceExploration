

#include "Scene/SelectTileScene/AC_MapTile_Battle.h"
#include "Manager/PlaySceneGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Scene/SelectTileScene/E_Tile.h"

// コンストラクタ
AAC_MapTileBattle::AAC_MapTileBattle() {

	tileType_ = E_TILE_TYPE::BATTLE;

}

// 開始時に実行する関数
void AAC_MapTileBattle::BeginPlay() {
	Super::BeginPlay();

	//移動先のレベルをパスで取得
	FSoftObjectPath nextlevelpath(TEXT("/Game/graphics/alpha/Master_Lv_battlemap/Lv_battle_map"));
	//パスで取得したレベルを設定
	NextLevel = TSoftObjectPtr<UWorld>(nextlevelpath);
}

// 更新用関数
void AAC_MapTileBattle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


}

// マスで実行するイベントの関数
// 一度だけ呼ぶ
// AAC_MapTileBaseクラスのTileEventクラスをオーバーライド
void AAC_MapTileBattle::TileEvent() {
	//バトルシーンに移動
	gamemode->ChangeLevel(NextLevel,this, false, false);

	//バトルで使用する敵のステータスをgameinstanceに

}