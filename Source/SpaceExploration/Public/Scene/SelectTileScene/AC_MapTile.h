// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "E_Tile.h"

#include "AC_MapTile.generated.h"

//********************************************************************
// �}�b�v��̃}�X�̃x�[�X�N���X
// ���ۃN���X�A���ꂼ��̃}�X�̃N���X�Ɍp�����ėp����
UCLASS( Abstract )
class SPACEEXPLORATION_API AAC_MapTileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_MapTileBase();

	// �}�X�̎�ނ̃Q�b�^�[
	inline E_TILE_TYPE getTileType() { return tileType_; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// �}�X�̎�ނ̕ϐ�
	UPROPERTY(EditAnywhere)
	E_TILE_TYPE tileType_ = E_TILE_TYPE::NONE;


	//------------------------------------------------------------------
	// ���b�V���֘A

	// SceneComponent
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> defaultSceneRoot_;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> staticMesh_;



public:	


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// �}�X�Ŏ��s����C�x���g�̊֐�
	// �p����ł��̊֐����I�[�o�[���C�h���ď������쐬����
	virtual void TileEvent() PURE_VIRTUAL(AAC_MapTileBase::TileEvent, );

	// �X�^�e�B�b�N���b�V�����Z�b�g����֐�
	// �����FfileName...�X�^�e�B�b�N���b�V���̕ۑ���̃p�X
	// 
	// �g�p��jSetStaticMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	void SetStaticMesh(const TCHAR* fileName);
};
