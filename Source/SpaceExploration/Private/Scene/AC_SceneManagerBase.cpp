// Fill out your copyright notice in the Description page of Project Settings.

#include "Scene/AC_SceneManagerBase.h"

#include "Character/PlayerCharacter.h"
#include "PlayerController/SelectPlanetPlayerController.h"
#include <Kismet/GameplayStatics.h>



// Sets default values
AAC_SceneManagerBase::AAC_SceneManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AAC_SceneManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
	// プレイヤーキャラクター取得
	playerCharacter_ = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!playerCharacter_) {
		UKismetSystemLibrary::PrintString(this, "PlayerCharacter_ is nullptr", true, true, FColor::Red, 3.f);
		UE_LOG(LogClass, Error, TEXT("AC_StageManager::BeginPlay...PlayerCharacter_ is nullptr"));
		return;
	}


	// シーケンスマネージャーを生成
	// ※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
	// 継承先で、ChangeSequence関数を用いて実行するデリゲートをセットすること！！！！！！！
	// ※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
	sequenceManager_ = NewObject<USequenceManager>();

	// sequenceManager_についてログ表示
	if (sequenceManager_ == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AC_StageManager::BeginPlay...sequenceManager_ is nullptr"));
		return;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("AC_StageManager::BeginPlay...sequenceManager_ generated!"));
	}



	// 自身をplayerControllerにセット
	Cast<ASelectPlanetPlayerController>(UGameplayStatics::GetPlayerController(this, 0))->SetStageMapManager(this);


}

// Called every frame
void AAC_SceneManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (sequenceManager_) {


		// シーケンスの更新
		sequenceManager_->updateSequence(DeltaTime);

		// 入力の状態を確認
		CheckClickInput();

	}


}


// -----------------------------------------------------------------------------------------------------

// レイを飛ばして当たったActorを取得する関数
AActor* AAC_SceneManagerBase::PerformRaycast() {

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




//------------------------------------------------------------------------------------
// 入力を確認する用の関数



// クリック入力を外部から伝えるための関数
// playerControllerでクリックされたときの処理としてバインドする用
void AAC_SceneManagerBase::OnClickInput() {

	// 入力が既にある場合は処理しない
	if (isClickInput_) { return; }

	// 入力があった状態にする
	isClickInput_ = true;

}


// クリック入力を確認する関数
// クリック入力を必要とする処理と、update関数で毎回isClickInputを確認する
bool AAC_SceneManagerBase::CheckClickInput() {

	// 入力の状態が入っていないときは処理しない
	if (!isClickInput_) { return false; }

	else {

		// 入力の状態をリセットする
		isClickInput_ = false;

		// trueを返す
		return true;
	}

}

