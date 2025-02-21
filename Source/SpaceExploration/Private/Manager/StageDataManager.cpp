// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/StageDataManager.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "NiagaraSystem.h"
#include "Manager/PlaySceneGameModeBase.h"

#include <Kismet/GameplayStatics.h>



// Sets default values
AStageDataManager::AStageDataManager()
	:tileDataClass_( UTileData::StaticClass() )
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// ランダム生成するためのクラスのコンポーネント生成
	galaxyRandomSelectComponent_ = CreateDefaultSubobject<UChildActorComponent>(TEXT("GalaxyRandomSelectComponent"));
	galaxyRandomSelectComponent_->SetupAttachment(RootComponent);
	galaxyRandomSelectComponent_->SetChildActorClass(AGalaxyRandomSelect::StaticClass());
	

	

}

// Called when the game starts or when spawned
void AStageDataManager::BeginPlay()
{
	Super::BeginPlay();

	// ゲームモード取得
	APlaySceneGameModeBase* playSceneGameMode = Cast<APlaySceneGameModeBase>( UGameplayStatics::GetGameMode( GetWorld() ) );

	// このクラスをゲームモードにセットする
	playSceneGameMode->SetStageDataManager(this);



	if (galaxyRandomSelectComponent_)
	{
		// 明示的に子アクターを生成
		galaxyRandomSelectComponent_->CreateChildActor();

		// 子オブジェクトを取得
		AActor* childActor = galaxyRandomSelectComponent_->GetChildActor();

		if (childActor)
		{
			galaxyRandomSelect_ = Cast<AGalaxyRandomSelect>(childActor);
			UE_LOG(LogTemp, Log, TEXT("子オブジェクトが正しく生成されました"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("子オブジェクトが取得できません"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("galaxyRandomSelectComponent_ が nullptr です"));
	}



	// 念のため空にしておく
	tileNiagaraArray_.Empty();
	// 指定したフォルダのナイアガラを読み込む
	GetNiagaraSystemsFromFolder("/Game/graphics/alpha/Master_planet/FX_planet_naiagara");


}

// Called every frame
void AStageDataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// =================================================================================================================
// ステージのデータの確認・取得を行う関数
// 
// 引数：stagaMapData...既に存在するデータを代入するための変数。
//		参照で渡すことで、データを入れることができるようにする
// 
// １．データが存在する場合は引数のstageMapDataにデータを入れたうえでtrue型を返す
// ２．データが存在しない場合はデータを入れずにfalse型を返す
bool AStageDataManager::TryGetStageMapData(FStageMapData& stageMapData)
{

	// マップデータが既に有効なとき
	if (stageMapData_.bIsValid_) {

		// 引数として渡した変数に有効なマップデータを入れる
		stageMapData = stageMapData_;
		
		return true;
	}

	// 有効なマップデータが存在しなかったときfalse
	return false;
}



// =================================================================================================================
// マスを移動する際に呼び出す関数
// 現在いるマス nowTilePosIndex_ を移動先のマスに変更する
// 次のマスに移動する際は呼び出すこと！
void AStageDataManager::MoveTile(const FVector2D& nextTileIndex)
{

	nowTilePosIndex_ = nextTileIndex;

}





// =================================================================================================================
// マスを追加で生成する関数
// 引数：tileNumArray...新しく生成するマスの配列の大きさ
// ----------------------------------------------------------------------
// 例）{ 1, 2, 3, 2 }とした場合、以下のようなマスを生成することを想定
// 3	　〇　〇	
// 2	〇　〇　〇
// 1	　〇　〇
// 0	　　〇
// ----------------------------------------------------------------------
void AStageDataManager::CreateTileArray(TArray<int> tileNumArray)
{
	// -----------------------------------------------------------------------------------
	// マスの種類をランダムで生成


	//if (galaxyRandomSelectComponent_)
	//{
	//	// 子オブジェクトを取得
	//	AActor* childActor = galaxyRandomSelectComponent_->GetChildActor();

	//	UE_LOG(LogTemp, Log, TEXT("コンポーネント確認"));

	//	if (childActor) {

	//		galaxyRandomSelect_ = Cast<AGalaxyRandomSelect>(childActor);

	//		UE_LOG(LogTemp, Log, TEXT("子オブジェクト確認"));

	//		if (galaxyRandomSelect_) {

	//			UE_LOG(LogTemp, Log, TEXT("マス種類生成"));

	//			// マスの種類の配列を作成
	//			tileTypeArray_ = galaxyRandomSelect_->MakeTileArray(tileNumArray);

	//		}


	//	}

	//}


	if (galaxyRandomSelect_) {

		UE_LOG(LogTemp, Log, TEXT("マス種類生成"));

		// マスの種類の配列を作成
		tileTypeArray_ = galaxyRandomSelect_->MakeTileArray(tileNumArray);

	}


	if (galaxyRandomSelect_ == nullptr) {
		UE_LOG(LogClass, Log, TEXT("galaxyRandomSelectがありません"));
	}


	// -----------------------------------------------------------------------------------
	// 種類に基づいてデータを作成

	// 保存されているナイアガラの数を確認
	int niagaraCount = tileNiagaraArray_.Num();


	for (int y = 0; y < tileTypeArray_.Num(); ++y) {

		// ステージデータに追加する用のマスの配列
		FTileDataArray newTileDataArray;
		

		for (int x = 0; x < tileTypeArray_[y].typeArray.Num(); ++x) {			
			
			// UTileData* newTileData = GetWorld()->SpawnActor<UTileData>(tileDataClass_);
			UTileData* newTileData = NewObject<UTileData>(this);

			//------------------------------------------------------------------
			// データをセット

			// ナイアガラが保存されているとき
			if (tileNiagaraArray_.Num() > 0) {
				


				// セットするナイアガラの番号を決定
				int setNiagaraNum = FMath::RandRange(0, niagaraCount - 1);


				if (tileNiagaraArray_[setNiagaraNum]) {

					// ナイアガラをセット
					newTileData->SetTileNiagaraSys(tileNiagaraArray_[setNiagaraNum]);

				
				}
				
			}

			// マスの種類セット
			newTileData->SetTileType(tileTypeArray_[y].typeArray[x]);		
			// マスの配列内での番号セット
			newTileData->SetTileArrayIndex( FVector2D( x, y ) );				

			// 配列に追加
			newTileDataArray.tileDataArray_.Add(newTileData);
		}

		// 配列をステージデータに追加
		stageMapData_.tileDataArray_.Add( newTileDataArray );

	}


	UE_LOG(LogTemp, Log, TEXT("stageDataManager::CreateTileArray stageMapData_.tileDataArray.Num() = %d"), stageMapData_.tileDataArray_.Num() );

	// データを有効にする
	stageMapData_.bIsValid_ = true;

	// ゲームモード取得
	APlaySceneGameModeBase* playSceneGameMode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	// 生成したステージのデータをゲームモードにセットする
	playSceneGameMode->SetStageMapData(&stageMapData_);


}



// =================================================================================================================
// 指定したフォルダ内のナイアガラを読み込んで保存する関数
// 
// 引数：FolderPath...ナイアガラの保存フォルダのパス
void AStageDataManager::GetNiagaraSystemsFromFolder(const FString& folderPath)
{
	// アセットレジストリを取得
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// フォルダ内のアセットを取得
	FARFilter Filter;
	Filter.PackagePaths.Add(*folderPath); // 指定したフォルダ内を検索
	Filter.bRecursivePaths = true;        // サブフォルダも検索
	Filter.ClassPaths.Add(UNiagaraSystem::StaticClass()->GetClassPathName()); // ナイアガラシステムのみ


	// フォルダ内のアセットを配列に入れる
	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssets(Filter, AssetDataList);

	UE_LOG(LogTemp, Log, TEXT("Found %d Niagara assets in folder: %s"), AssetDataList.Num(), *folderPath);


	// ナイアガラシステムを配列に追加
	for (const FAssetData& AssetData : AssetDataList)
	{
		TSoftObjectPtr<UNiagaraSystem> SoftNiagaraSystem = TSoftObjectPtr<UNiagaraSystem>(AssetData.ToSoftObjectPath());

		// ソフトオブジェクトを同期ロード
		UNiagaraSystem* LoadedNiagaraSystem = SoftNiagaraSystem.LoadSynchronous();

		if (LoadedNiagaraSystem)
		{
			// TObjectPtr に格納
			TObjectPtr<UNiagaraSystem> NiagaraSystem = LoadedNiagaraSystem;
			tileNiagaraArray_.Add(NiagaraSystem);
		}
	}

}


