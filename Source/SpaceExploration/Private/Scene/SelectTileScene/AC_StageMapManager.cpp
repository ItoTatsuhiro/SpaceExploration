// Fill out your copyright notice in the Description page of Project Settings.

#include "Scene/SelectTileScene/AC_StageMapManager.h"

#include "../../../Public/Scene/SelectTileScene/AC_MapTile.h"
#include "../../../Public/Scene/SelectTileScene/AC_MapTileHeal.h"
#include "../../../Public/Scene/SelectTileScene/AC_MapTileItem.h"
#include "../../../Public/Scene/SelectTileScene/AC_MapTile_Battle.h"
#include "../../../Public/Character/PlayerCharacter.h"
#include "PlayerController/SelectPlanetPlayerController.h"
#include "Manager/StageDataManager.h"
#include "../../../Public/Manager/PlaySceneGameModeBase.h"
#include "GameData/TileData.h"

#include <Kismet/GameplayStatics.h>





// Sets default values
AAC_StageMapManager::AAC_StageMapManager()
    : tileSpace_(500), basePos_({ 0, 0, 0 })/*, sequenceManager_(nullptr)*/, galaxyRandomSelect_(nullptr), galaxyRandomSelectComponent_(nullptr)/*, tileObjectComponent_(nullptr)*/, hoveredTile_(nullptr),
    battleTileClass_(AAC_MapTileBattle::StaticClass()), healTileClass_(AAC_MapTileHeal::StaticClass()), itemTileClass_(AAC_MapTileItem::StaticClass())
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;




    // ランダム生成するためのクラスのコンポーネント生成
    galaxyRandomSelectComponent_ = CreateDefaultSubobject<UChildActorComponent>(TEXT("GalaxyRandomSelectComponent"));
    galaxyRandomSelectComponent_->SetChildActorClass(AGalaxyRandomSelect::StaticClass());
    galaxyRandomSelectComponent_->SetupAttachment(RootComponent);


    //tileObjectComponent_ = CreateDefaultSubobject<UChildActorComponent>(TEXT("tileObjectComponent"));
    //tileObjectComponent_->SetupAttachment(RootComponent);


}




// Called when the game starts or when spawned
void AAC_StageMapManager::BeginPlay()
{

    Super::BeginPlay();

    //-------------------------------------------------------------------------------
    // シーケンス制御用の処理

    // デリゲート初期化
    createTileDel_ = FSequenceDelegate::CreateUObject(this, &AAC_StageMapManager::SeqCreateTile);
    selectTileDel_ = FSequenceDelegate::CreateUObject(this, &AAC_StageMapManager::SeqSelectTile);
    playerMoveBeginDel_ = FSequenceDelegate::CreateUObject(this, &AAC_StageMapManager::SeqPlayerMoveBegin);
    playerMoveIdleDel_ = FSequenceDelegate::CreateUObject(this, &AAC_StageMapManager::SeqPlayerMoveIdle);
    startTileEventDel_ = FSequenceDelegate::CreateUObject(this, &AAC_StageMapManager::SeqStartTileEvent);
    tileEventProcessDel_ = FSequenceDelegate::CreateUObject(this, &AAC_StageMapManager::SeqTileEventProcess);



    // シーケンスマネージャーが存在するときは初期シーケンスのデリゲートをセット
    if (sequenceManager_) {
        sequenceManager_->ChangeSequence(createTileDel_);
    }


    //和田記載-----------------------------------------------------------

    playercontroller = UGameplayStatics::GetPlayerController(this, 0);
    //カメラ切り替え
    if (playerlookingdowncamera == nullptr) {
        //playercamera = Cast<AActor>(playerCharacter_->GetBattleCameraComponent());
        playerlookingdowncamera = playerCharacter_->GetLookingDownCaemeraComponent()->GetChildActor();

        if (playerlookingdowncamera == nullptr) {
            UE_LOG(LogClass, Warning, TEXT("AAC_StageMapManager::BeginPlay : error : No playerCamera\n"));
        }
    }


    playercontroller->SetViewTargetWithBlend(playerlookingdowncamera, 0.0f);

    //-------------------------------------------------------------------
}



void AAC_StageMapManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {

    AAC_SceneManagerBase::EndPlay(EndPlayReason);

    int destroyCount = 0;

    for (int i = tileObjArray_.Num() - 1; i >= 0; --i) {

        int exeCount = tileObjArray_[i].TileArray.Num();

        for (int j = exeCount - 1; j >= 0; --j) {

            tileObjArray_[i].TileArray[j]->Destroy();

            tileObjArray_[i].TileArray.RemoveAt(j);

            ++destroyCount;



        }

    }

    tileObjArray_.Empty();


    UE_LOG(LogClass, Log, TEXT("AAC_StageMapManager::EndPlay : EndPlay\n"));

    return;
}



// Called every frame
void AAC_StageMapManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);




}





// -----------------------------------------------------------------------------------------------------
// シーケンス用

// マス生成シーケンス
void AAC_StageMapManager::SeqCreateTile(const float delta_time) {

    if (sequenceManager_ == nullptr) {
        return;
    }


    //// マスの種類をランダムで生成
    //if (galaxyRandomSelectComponent_)
    //{
    //    // 子オブジェクトを取得
    //    AActor* childActor = galaxyRandomSelectComponent_->GetChildActor();

    //    UE_LOG(LogTemp, Log, TEXT("コンポーネント確認"));

    //    if (childActor) {

    //        galaxyRandomSelect_ = Cast<AGalaxyRandomSelect>(childActor);

    //        UE_LOG(LogTemp, Log, TEXT("子オブジェクト確認"));

    //        if (galaxyRandomSelect_) {

    //            UE_LOG(LogTemp, Log, TEXT("オブジェクト生成"));

    //            CreateTileObjArray({ 1, 2, 3, 2, 3, 2, 1 });
    //        }
    //    }
    //}


    // ゲームモード
    APlaySceneGameModeBase* playsceneGameMode = Cast<APlaySceneGameModeBase>( UGameplayStatics::GetGameMode( GetWorld() ) );

    // マップのデータの取得を試みる
    if ( !(playsceneGameMode->GetStageDataManager()->TryGetStageMapData(stageMapData_)) ) {

        UE_LOG(LogTemp, Log, TEXT("stageMapManager::SeqCreateTile　タイルデータ作成"));
        playsceneGameMode->GetStageDataManager()->CreateTileArray({ 1, 2, 3, 2, 3, 2, 1 });

        playsceneGameMode->GetStageDataManager()->TryGetStageMapData(stageMapData_);
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("stageMapManager::SeqCreateTile　タイルデータ取得"));
    }

    // stageMapData_の内容をもとにオブジェクトを作成する
    CreateTileObjArray({ 1, 2, 3, 2, 3, 2, 1 });





    // 実行するシーケンスを切り替え
    // 切り替え先：マス選択シーケンス
    sequenceManager_->ChangeSequence(selectTileDel_);

    UE_LOG(LogTemp, Log, TEXT("シーケンス切り替え：selectTileDel_"));

}





// マス選択シーケンス
void AAC_StageMapManager::SeqSelectTile(const float delta_time) {

    // カーソルと重なっているオブジェクトを取得
    AActor* hoveredObj = PerformRaycast();

    // 重なっているオブジェクトがマスのクラスを継承している場合
    if (hoveredObj && hoveredObj->IsA(AAC_MapTileBase::StaticClass())) {

        // 重なっているオブジェクトをMapTileBaseのクラスにキャスト
        AAC_MapTileBase* hoveredTile = Cast<AAC_MapTileBase>(hoveredObj);

        // 現在変数として置いてあるものと比較
        if (hoveredTile_ != hoveredTile && hoveredTile) {

            // 置き換え
            hoveredTile_ = hoveredTile;

            // ログを表示
            UE_LOG(LogTemp, Log, TEXT("HoveredTile: %s"), *hoveredTile_->GetName());
        }

    }
    else {
        hoveredTile_ = nullptr;
    }

    // デバッグ用に重なっているマスに目印を付ける
    if (hoveredTile_ != nullptr) {

        DrawDebugSphere(GetWorld(), hoveredTile_->GetActorLocation(), 100.0f, 12, FColor::Red, false);
    }

    // クリックされていない状態の時は処理しない
    if (!CheckClickInput()) { return; }
    // 重なっているマスが存在しない場合は処理しない
    if (!hoveredTile_) { return; }

    // ログを表示
    UE_LOG(LogTemp, Log, TEXT("クリック"));

    // 重なっているマスを選択中のマスとして登録
    selectTile_ = hoveredTile_;

    // 実行するシーケンスを切り替え
    // 切り替え先：移動開始シーケンス
    sequenceManager_->ChangeSequence(playerMoveBeginDel_);

}




// プレイヤー移動シーケンス
void AAC_StageMapManager::SeqPlayerMoveBegin(const float delta_time) {


    // プレイヤーの移動を行う
    // プレイヤーから、クリックした際の処理を取ってくる


    // マスが選択されていないときは処理しない
    if (!selectTile_) {

        // ログを表示
        UE_LOG(LogTemp, Log, TEXT("マスが選択されていません"));

        return;
    }




    // プレイヤーが待機中でないときは処理しない
    if (playerCharacter_->GetCharaterActState() != ECharacterActState::Idle) {
        return;
    }


    // 移動先の座標を計算
    FVector movePos = selectTile_->GetActorLocation() + playerCharacter_->GetActorRotation().RotateVector(moveTargetOffset_);

    // 移動開始の指示
    playerCharacter_->BeginMoveTargetLocation(movePos);



    // 実行するシーケンスを切り替え
    // 切り替え先：移動終了待機シーケンス
    sequenceManager_->ChangeSequence(playerMoveIdleDel_);


}





// プレイヤー移動待機シーケンス
void AAC_StageMapManager::SeqPlayerMoveIdle(const float delta_time) {


    // 移動が終了した待機状態でないときは処理しない
    if (playerCharacter_->GetCharaterActState() != ECharacterActState::Idle) {
        return;
    }


    // 移動終了後

    // 実行するシーケンスを切り替え
    // 切り替え先：マスのイベント実行シーケンス
    sequenceManager_->ChangeSequence(startTileEventDel_);

}




// マスのイベント実行シーケンス
void AAC_StageMapManager::SeqStartTileEvent(const float delta_time) {


    // マスのイベントを実行する処理（仮）


    // マスが選択されていないときは処理しない
    if (!selectTile_) {

        // ログを表示
        UE_LOG(LogTemp, Log, TEXT("マスが選択されていません"));

        return;
    }


    // **********************************
    // イベント実行処理
    // **********************************

    // マスのイベントを実行
    selectTile_->TileEvent();




    // ※※※ イベント実行後 ※※※

    // 実行するシーケンスを切り替え
    // 切り替え先：マス選択シーケンス
    sequenceManager_->ChangeSequence(tileEventProcessDel_);



}

void AAC_StageMapManager::SeqTileEventProcess(const float)
{
    selectTile_->TileEventRunning();

    if (!selectTile_->IsEventCompleted())
    {
        return;
    }

    sequenceManager_->ChangeSequence(selectTileDel_);

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

    //if (!galaxyRandomSelect_) {
    //    UE_LOG(LogTemp, Log, TEXT("stageMapManager::CreateTileObjArray　galaxyRandomSelectがありません"));
    //    return;
    //}


    // ゲームモード
    APlaySceneGameModeBase* playsceneGameMode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));



    UE_LOG(LogTemp, Log, TEXT("stageMapManager::CreateTileObjArray　createTileNumArray.Num() = %d"), createTileNumArray.Num());


    // playsceneGameMode->GetStageDataManager()->CreateTileArray(createTileNumArray);

    // playsceneGameMode->GetStageDataManager()->TryGetStageMapData(stageMapData_);

    //// マスの種類をランダムで生成
    //tileTypeArray_ = galaxyRandomSelect_->MakeTileArray(createTileNumArray);



    UE_LOG(LogTemp, Log, TEXT("stageMapManager::CreateTileObjArray　stageMapData_.tileDataArray_[row].Num() = %d"), stageMapData_.tileDataArray_.Num());




    // tileTypeArray_の中身に応じてそれぞれのマスのオブジェクトを生成し、
    // tileObjArray_に入れる処理
    for (int row = 0; row < stageMapData_.tileDataArray_.Num(); ++row)
    {
        // 二重配列に入れる用の仮の配列
        FTileArray tempTileArray;

        AAC_MapTileBase* tempTile;




        //// マスのデータを元にオブジェクトを生成する！！！！！！
        //stageMapData_.tileDataArray_[row].tileDataArray_[col]->GetTileType();



        UE_LOG(LogTemp, Log, TEXT("stageMapManager::CreateTileObjArray　stageMapData_.tileDataArray_[row].tileDataArray_.Num() = %d"), stageMapData_.tileDataArray_[row].tileDataArray_.Num());



        // マスのオブジェクトを生成
        for (int col = 0; col < stageMapData_.tileDataArray_[row].tileDataArray_.Num(); ++col)
        {
            // マスの種類に応じて実際のオブジェクトを生成する処理
            switch ( stageMapData_.tileDataArray_[row].tileDataArray_[col]->GetTileType() )
            {
            case E_TILE_TYPE::BATTLE:
                tempTile = GetWorld()->SpawnActor<AAC_MapTileBattle>(battleTileClass_);

                
                break;

            case E_TILE_TYPE::ITEM:
                tempTile = GetWorld()->SpawnActor<AAC_MapTileItem>(itemTileClass_);

                break;

            case E_TILE_TYPE::HEAL:

                tempTile = GetWorld()->SpawnActor<AAC_MapTileHeal>(healTileClass_);
                break;

            case E_TILE_TYPE::NONE:
                tempTile = GetWorld()->SpawnActor<AAC_MapTileBattle>(battleTileClass_);



                UE_LOG(LogTemp, Log, TEXT("tileTypeArray_[%d].typeArray[%d] = NONE"), row, col);
                break;

            default:
                tempTile = GetWorld()->SpawnActor<AAC_MapTileBattle>(battleTileClass_);


                UE_LOG(LogTemp, Log, TEXT("tileTypeArray_[%d].typeArray[%d] = OTHER"), row, col);

                break;
            }


            // データに設定されているのナイアガラシステムをオブジェクト自身に設定
            tempTile->SetNiagaraSystem( stageMapData_.tileDataArray_[row].tileDataArray_[col]->GetTileNiagaraSys() );


            // 配列に追加
            tempTileArray.TileArray.Emplace(tempTile);

            // 
            tempTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);


        }

        // 二重配列に入れる
        tileObjArray_.Emplace(tempTileArray);
    }

    UE_LOG(LogTemp, Log, TEXT("マス生成完了"));


    // マスをそれぞれ順番に配置
    for (int row = 0; row < tileObjArray_.Num(); ++row)
    {

        for (int col = 0; col < tileObjArray_[row].TileArray.Num() ; ++col)
        {


            // 中身が無ければログを表示
            if (tileObjArray_[row].TileArray[col] == nullptr) {

                UE_LOG(LogTemp, Log, TEXT("tileObjArray_[%d].TileArray[%d] = nullptr"), row, col);

                continue;
            }


            // x方向の位置
            float colPos = col - ((tileObjArray_[row].TileArray.Num() - 1) / 2.0f);

            // マスを配置する座標を計算
            FVector tilePos = basePos_ + FVector{ colPos, static_cast<float>(row), 0.0f } *tileSpace_;

            UE_LOG(LogTemp, Log, TEXT("tile(%d, %d) pos = { %d, %d }"), row, col, tilePos.X, tilePos.Y);

            // 座標を設定
            tileObjArray_[row].TileArray[col]->SetActorLocation(tilePos);
        }
    }
}
