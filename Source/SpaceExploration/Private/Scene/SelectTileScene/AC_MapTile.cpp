// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SelectTileScene/AC_MapTile.h"

// Sets default values
AAC_MapTileBase::AAC_MapTileBase()
	: isTileEventEnd(false), tileType_(E_TILE_TYPE::NONE)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//---------------------------------------------------------------------------------------
	// メッシュ関係の処理

	// SceneComponentを作成
	defaultSceneRoot_ = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// SceneComponentをRootComponentに設定
	RootComponent = defaultSceneRoot_;


	// スタティックメッシュを作成
	staticMesh_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// スタティックメッシュのセット
	SetStaticMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));

	// スタティックメッシュのアタッチ
	staticMesh_->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AAC_MapTileBase::BeginPlay()
{

	Super::BeginPlay();
	
}


//void AAC_MapTile::


// Called every frame
void AAC_MapTileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	TileEvent();

}


// スタティックメッシュをセットする関数
// 引数：fileName...スタティックメッシュの保存先のパス
// 
// 使用例）SetStaticMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
void AAC_MapTileBase::SetStaticMesh(const TCHAR* fileName) {


	// 引数のスタティックメッシュを設定
	UStaticMesh* mesh = LoadObject<UStaticMesh>(NULL, fileName, NULL, LOAD_None, NULL);
	staticMesh_->SetStaticMesh(mesh);


}