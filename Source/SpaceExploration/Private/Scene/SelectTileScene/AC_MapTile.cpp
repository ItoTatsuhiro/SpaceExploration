// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SelectTileScene/AC_MapTile.h"

// Sets default values
AAC_MapTileBase::AAC_MapTileBase()
	: isTileEventEnd(false), tileType_(E_TILE_TYPE::NONE)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//---------------------------------------------------------------------------------------
	// ���b�V���֌W�̏���

	// SceneComponent���쐬
	defaultSceneRoot_ = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// SceneComponent��RootComponent�ɐݒ�
	RootComponent = defaultSceneRoot_;


	// �X�^�e�B�b�N���b�V�����쐬
	staticMesh_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// �X�^�e�B�b�N���b�V���̃Z�b�g
	SetStaticMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));

	// �X�^�e�B�b�N���b�V���̃A�^�b�`
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


// �X�^�e�B�b�N���b�V�����Z�b�g����֐�
// �����FfileName...�X�^�e�B�b�N���b�V���̕ۑ���̃p�X
// 
// �g�p��jSetStaticMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
void AAC_MapTileBase::SetStaticMesh(const TCHAR* fileName) {


	// �����̃X�^�e�B�b�N���b�V����ݒ�
	UStaticMesh* mesh = LoadObject<UStaticMesh>(NULL, fileName, NULL, LOAD_None, NULL);
	staticMesh_->SetStaticMesh(mesh);


}