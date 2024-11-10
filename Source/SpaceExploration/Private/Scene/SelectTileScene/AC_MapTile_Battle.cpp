

#include "Scene/SelectTileScene/AC_MapTile_Battle.h"
#include "Kismet/GameplayStatics.h"
#include "Scene/SelectTileScene/E_Tile.h"

// コンストラクタ
AAC_MapTileBattle::AAC_MapTileBattle() {

	tileType_ = E_TILE_TYPE::BATTLE;

}

// 開始時に実行する関数
void AAC_MapTileBattle::BeginPlay() {
	Super::BeginPlay();

	//親レベルにあるALevelInterfaceクラスを取得
	levelinterface = Cast<ALevelInterface>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelInterface::StaticClass()));
	if (!levelinterface) {
		UE_LOG(LogTemp, Warning, TEXT("not ALevelInterface\n"));
	}
}

// 更新用関数
void AAC_MapTileBattle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


}

// マスで実行するイベントの関数
// 一度だけ呼ぶ
// AAC_MapTileBaseクラスのTileEventクラスをオーバーライド
void AAC_MapTileBattle::TileEvent() {
	//バトルシーン生成

	//バトルで使用する敵のステータスをgameinstanceに

}