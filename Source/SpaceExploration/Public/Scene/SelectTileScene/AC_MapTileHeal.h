// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scene/SelectTileScene/AC_MapTile.h"
#include "AC_MapTileHeal.generated.h"

// �񕜂��s���}�X�̃N���X
// �񕜏����͌�X
UCLASS(BlueprintType)
class SPACEEXPLORATION_API AAC_MapTileHeal : public AAC_MapTileBase
{
	GENERATED_BODY()
	

public:

	// �R���X�g���N�^
	AAC_MapTileHeal();


private:

	// �J�n���Ɏ��s����֐�
	void BeginPlay() override;


public:


	// �X�V�p�֐�
	void Tick(float DeltaTime) override;


	// �}�X�Ŏ��s����C�x���g�̊֐�
	// AAC_MapTileBase�N���X��TileEvent�N���X���I�[�o�[���C�h
	void TileEvent() override;

};