// Fill out your copyright notice in the Description page of Project Settings.

#include "Scene/SelectTileScene/AC_StageMapManager.h"

#include "../../../Public/Scene/SelectTileScene/AC_MapTile.h"
#include "../../../Public/Scene/SelectTileScene/AC_MapTileHeal.h"
#include "../../../Public/Scene/SelectTileScene/AC_MapTileItem.h"
#include "../../../Public/Scene/SelectTileScene/AC_MapTile_Battle.h"




// Sets default values
AAC_StageMapManager::AAC_StageMapManager()
    : tileSpace_(500), basePos_({ 0, 0, 0 }),
    battleTileClass_(AAC_MapTileBattle::StaticClass()), healTileClass_(AAC_MapTileHeal::StaticClass()), itemTileClass_(AAC_MapTileItem::StaticClass())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



    // ランダム生成するためのクラスのコンポーネント生成
    galaxyRandomSelectComponent_ = CreateDefaultSubobject<UChildActorComponent>(TEXT("GalaxyRandomSelectComponent"));
    galaxyRandomSelectComponent_->SetChildActorClass(AGalaxyRandomSelect::StaticClass());
    galaxyRandomSelectComponent_->SetupAttachment(RootComponent);




    tileObjectComponent_ = CreateDefaultSubobject<UChildActorComponent>(TEXT("tileObjectComponent"));
    tileObjectComponent_->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AAC_StageMapManager::BeginPlay()
{

	Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("BeginPlay"));

    // マスの種類をランダムで生成
    if (galaxyRandomSelectComponent_)
    {
        // 子オブジェクトを取得
        AActor* childActor = galaxyRandomSelectComponent_->GetChildActor();

        UE_LOG(LogTemp, Log, TEXT("コンポーネント確認"));

        if (childActor) {

            galaxyRandomSelect_ = Cast<AGalaxyRandomSelect>(childActor);

            UE_LOG(LogTemp, Log, TEXT("子オブジェクト確認"));

            if (galaxyRandomSelect_) {

                // tileTypeArray_ = galaxyRandomSelect_->MakeTileArray({ 1, 2, 3, 2, 3, 2, 1 });

                UE_LOG(LogTemp, Log, TEXT("オブジェクト生成"));

                CreateTileObjArray({ 1, 2, 3, 2, 3, 2, 1 });
            }


        }



    }

    // CreateTileObjArray();

}

// Called every frame
void AAC_StageMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// マスを追加で生成する関数
// 引数：createTileNumArray...新しく生成するマスの配列の大きさ
// ----------------------------------------------------------------------
// 例）{ 1, 2, 3, 2 }とした場合、以下のようなマスを生成することを想定
// 3	　〇　〇	
// 2	〇　〇　〇
// 1	　〇　〇
// 0	　　〇
// ----------------------------------------------------------------------
void AAC_StageMapManager::CreateTileObjArray(TArray<int> createTileNumArray)
{
    // 既にある配列の中身を削除
    tileTypeArray_.Empty();
    tileObjArray_.Empty();

    if (!galaxyRandomSelect_) {
        return;
    }


    if (!tileObjectComponent_) {
        // 生成するマスを保持するためのコンポーネント作成
        tileObjectComponent_ = CreateDefaultSubobject<UChildActorComponent>(TEXT("tileObjectComponent"));
        tileObjectComponent_->SetupAttachment(RootComponent);
    }



    // マスの種類をランダムで生成
    tileTypeArray_ = galaxyRandomSelect_->MakeTileArray(createTileNumArray);

    // tileTypeArray_の中身に応じてそれぞれのマスのオブジェクトを生成し、
    // tileObjArray_に入れる処理
    for (int row = 0; row < tileTypeArray_.Num(); ++row)
    {
        // 二重配列に入れる用の仮の配列
        FTileArray tempTileArray;

        // マスのオブジェクトを生成
        for (int col = 0; col < tileTypeArray_[row].typeArray.Num(); ++col)
        {
            // マスの種類に応じて実際のオブジェクトを生成する処理
            switch (tileTypeArray_[row].typeArray[col])
            {
            case E_TILE_TYPE::BATTLE:
                tempTileArray.TileArray.Emplace(GetWorld()->SpawnActor<AAC_MapTileBattle>(battleTileClass_));
                break;

            case E_TILE_TYPE::ITEM:
                tempTileArray.TileArray.Emplace(GetWorld()->SpawnActor<AAC_MapTileItem>(itemTileClass_));
                break;

            case E_TILE_TYPE::HEAL:
                tempTileArray.TileArray.Emplace(GetWorld()->SpawnActor<AAC_MapTileHeal>(healTileClass_));
                break;

            case E_TILE_TYPE::NONE:
                // NONEのときはBattleのクラスを生成するようにしておく
                tempTileArray.TileArray.Emplace(GetWorld()->SpawnActor<AAC_MapTileBattle>(battleTileClass_));

                UE_LOG(LogTemp, Log, TEXT("tileTypeArray_[%d].typeArray[%d] = NONE"), row, col);
                break;

            default:
                // 何にも当てはまらない場合は念のためBattleのクラスを生成するようにしておく
                tempTileArray.TileArray.Emplace(GetWorld()->SpawnActor<AAC_MapTileBattle>(battleTileClass_));

                UE_LOG(LogTemp, Log, TEXT("tileTypeArray_[%d].typeArray[%d] = OTHER"), row, col);

                break;
            }
        }

        // 二重配列に入れる
        tileObjArray_.Emplace(tempTileArray);
    }

    // **********************************************************
    // マスのオブジェクトを生成するところまではできているので、
    // それぞれのマスのオブジェクトの座標を設定するところから開始する！
    // **********************************************************
    for (int row = 0; row < tileTypeArray_.Num(); ++row)
    {


        for (int col = 0; col < tileTypeArray_[row].typeArray.Num(); ++col)
        {

            if (tileObjArray_[row].TileArray[col] == nullptr) {

                UE_LOG(LogTemp, Log, TEXT("tileObjArray_[%d].TileArray[5d] = nullptr"), row, col);

                continue;
            }


            // x方向の割合
            float colRatio = static_cast<float>(col + 1) / tileObjArray_[row].TileArray.Num();

            // マスを配置する座標を計算
            FVector tilePos = basePos_ + FVector{ (1.0f - 1.0f / (static_cast<float>(col + 1) * 2.0f)) * colRatio, static_cast<float>(row), 0.0f } *tileSpace_;

            // 座標を設定
            tileObjArray_[row].TileArray[col]->SetActorLocation(tilePos);
        }
    }
}