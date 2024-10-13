// Fill out your copyright notice in the Description page of Project Settings.

#include "Scene/SelectTileScene/AC_StageMapManager.h"

#include "../../../Public/Scene/SelectTileScene/AC_MapTile.h"
#include "../../../Public/Scene/SelectTileScene/AC_MapTileHeal.h"
#include "../../../Public/Scene/SelectTileScene/AC_MapTileItem.h"
#include "../../../Public/Scene/SelectTileScene/AC_MapTile_Battle.h"
#include "../../../Public/Character/PlayerCharacter.h"

#include <Kismet/GameplayStatics.h>





// Sets default values
AAC_StageMapManager::AAC_StageMapManager()
    : tileSpace_(500), basePos_({ 0, 0, 0 }), sequenceManager_(nullptr), galaxyRandomSelect_(nullptr), galaxyRandomSelectComponent_(nullptr), tileObjectComponent_(nullptr), hoveredTile_(nullptr),
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

    PlayerCharacter_ = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (!PlayerCharacter_) {
        UKismetSystemLibrary::PrintString(this, "PlayerCharacter_ is nullptr", true, true, FColor::Red, 3.f);
        UE_LOG(LogClass, Error, TEXT("PlayerCharacter_ is nullptr"));
        return;
    }


    //-------------------------------------------------------------------------------
    // シーケンス制御用の処理

    // デリゲート初期化
    createTileDel_ = FSequenceDelegate::CreateUObject(this, &AAC_StageMapManager::SeqCreateTile);
    selectTileDel_ = FSequenceDelegate::CreateUObject(this, &AAC_StageMapManager::SeqSelectTile);

    // 最初に実行する関数を指定
    sequenceManager_ = NewObject<USequenceManager>();

    if (sequenceManager_ == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("sequenceManager_ is nullptr"));
        return;
    }

    sequenceManager_->ChangeSequence(createTileDel_);

}

// Called every frame
void AAC_StageMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



    if (sequenceManager_) {


        // シーケンスの更新
        sequenceManager_->updateSequence(DeltaTime);

    }
    else {

        UE_LOG(LogTemp, Error, TEXT("sequenceManager_ is nullptr"));
    }

}


// -----------------------------------------------------------------------------------------------------
// シーケンス用

// マス生成シーケンス
void AAC_StageMapManager::SeqCreateTile(const float delta_time) {

    if (sequenceManager_ == nullptr) {
        return;
    }


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

                UE_LOG(LogTemp, Log, TEXT("オブジェクト生成"));

                CreateTileObjArray({ 1, 2, 3, 2, 3, 2, 1 });
            }

        }

    }



    // 実行するシーケンスを切り替え
    // 切り替え先：マス選択シーケンス
    sequenceManager_->ChangeSequence(selectTileDel_);
    
    UE_LOG(LogTemp, Log, TEXT("シーケンス切り替え：selectTileDel_"));

}





// マス選択シーケンス
void AAC_StageMapManager::SeqSelectTile(const float delta_time) {

    // カーソルと重なっているオブジェクトを取得
    AActor* hoveredObj = PerformRaycast();

    if ( hoveredObj != nullptr && hoveredObj->IsA(AAC_MapTileBase::StaticClass())) {

        // 重なっているオブジェクトをMapTileBaseのクラスにキャスト
        AActor* hoveredTile = Cast<AAC_MapTileBase>(hoveredObj);

        // 現在変数として置いてあるものと比較
        if (hoveredTile_ != hoveredTile) {
            
            // 置き換え
            hoveredTile_ = hoveredTile;

            // ログを表示
            UE_LOG(LogTemp, Log, TEXT("HoveredTile: %s"), *hoveredTile_->GetName() );
        }

    }
    else {

        hoveredTile_ = nullptr;

    }

    // デバッグ用に重なっているマスに目印を付ける
    if (hoveredTile_ != nullptr) {

        DrawDebugSphere(GetWorld(), hoveredTile_->GetActorLocation(), 100.0f, 12, FColor::Red, false);

    }




}





// -----------------------------------------------------------------------------------------------------
// マス生成関係

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

    // マスをそれぞれ順番に配置
    for (int row = 0; row < tileTypeArray_.Num(); ++row)
    {

        for (int col = 0; col < tileTypeArray_[row].typeArray.Num(); ++col)
        {
            // 中身が無ければログを表示
            if (tileObjArray_[row].TileArray[col] == nullptr) {

                UE_LOG(LogTemp, Log, TEXT("tileObjArray_[%d].TileArray[5d] = nullptr"), row, col);

                continue;
            }


            // x方向の位置
            float colPos = col - ( (tileObjArray_[row].TileArray.Num() - 1) / 2.0f );
            
            // マスを配置する座標を計算
            FVector tilePos = basePos_ + FVector{ colPos, static_cast<float>(row), 0.0f} * tileSpace_;


            // 座標を設定
            tileObjArray_[row].TileArray[col]->SetActorLocation(tilePos);
        }
    }
}




// -----------------------------------------------------------------------------------------------------
// クリックでオブジェクトを取得するための関数

// レイを飛ばして当たったActorを取得する関数
AActor* AAC_StageMapManager::PerformRaycast() {

    // ----------------------------------------------------------------------
    // レイの開始地点と終了地点を定義

    // プレイヤーコントローラー取得
    APlayerController* playerController = GetWorld()->GetFirstPlayerController();
    if (!playerController) {
        return nullptr;
    }


    // カーソルの位置のオブジェクトを取得
    FHitResult hitResult;
    if (playerController->GetHitResultUnderCursor(ECC_Visibility, false, hitResult)) {
        AActor* hitActor = hitResult.GetActor();

        if (hitActor) {

             //UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *hitActor->GetName());

            // デバッグ用にヒット位置を表示
            DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 10.0f, 12, FColor::Red, false, 1.0f);


            return hitActor;
        }

    }

    return nullptr;

}