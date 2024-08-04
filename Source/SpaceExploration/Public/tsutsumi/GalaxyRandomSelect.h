// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Scene/SelectTileScene/AC_MapTile.h"
#include "../Scene/SelectTileScene/E_Tile.h"
#include "GalaxyRandomSelect.generated.h"

UCLASS(BlueprintType)
class SPACEEXPLORATION_API AGalaxyRandomSelect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGalaxyRandomSelect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// �����_���ȃ}�X�̎�ނ�Ԃ��֐�
	// �i�ɓ��ꕔ�ύX�j
	E_TILE_TYPE RandomTileSelect();

	// �z���̃����_���Ȉʒu�Ɉ����̎�ނ̃}�X��}������֐�
	void RandomInsertTileType(E_TILE_TYPE insertTile);

	// ��������}�X�̔z��
	// �i�ɓ��ǉ��j
	TArray<TArray<E_TILE_TYPE>> tileArray_;

	// �e��ɂ�����}�X�̐�
	// ���̐����}�X�𐶐�����
	// �R���X�g���N�^�ŏ�����
	// �i�ɓ��ǉ��j
	UPROPERTY(EditAnywhere)
	TArray<int> tileNumHolizontal_;


	// �}�X�̎�ނ̐��A�R���X�g���N�^�Őݒ�
	int tileTypeNum_;

	// �}�X�S�̂̐�
	int tileCount_;

	// �}�X�̐��������m��
	UPROPERTY(EditAnywhere)
	TArray<float> selectWeight_ = { 40.0f, 35.0f, 25.0f };

	// ���ꂼ��̃}�X���Œ���������鐔
	UPROPERTY(EditAnywhere)
	TArray<int> minSelectNum_ = { 1, 1, 1 };


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// �}�X�̔z����쐬����֐�
	// ���̊֐����Ăяo���Ĕz����쐬����
	// �i�ɓ��ǉ��j
	void MakeTileArray();

	// �z��̓Y���������炷�֐�
	// ���炵����Ƀ}�X�����邩�m�F���A�}�X������ʒu�܂ł��炷
	FVector2D CheckShiftArray(FVector2D before);


	//----------------------------------------------------------------------------------------
	// �Q�b�^�[�Z�b�^�[

	// �}�X�̔z��̃Q�b�^�[
	// �i�ɓ��ǉ��j
	TArray<TArray<E_TILE_TYPE>> GetTileArray() const { return tileArray_; }



	////�f���̔z��
	//E_TILE_TYPE PlanetLine[3][3];	


private:
	// �K�v�ɉ����āA�v���C�x�[�g�����o�[��ǉ����Ă�������
};
