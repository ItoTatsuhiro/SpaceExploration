// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SelectTileScene/AC_MapTileHeal.h"

#include "Scene/SelectTileScene/E_Tile.h"



// �R���X�g���N�^
AAC_MapTileHeal::AAC_MapTileHeal() {


	tileType_ = E_TILE_TYPE::HEAL;

}


// �J�n���Ɏ��s����֐�
void AAC_MapTileHeal::BeginPlay() {



}



// �X�V�p�֐�
void AAC_MapTileHeal::Tick(float DeltaTime) {



}


// �}�X�Ŏ��s����C�x���g�̊֐�
// AAC_MapTileBase�N���X��TileEvent�N���X���I�[�o�[���C�h
void AAC_MapTileHeal::TileEvent() {

	int32 RecoverHP(100000);

}