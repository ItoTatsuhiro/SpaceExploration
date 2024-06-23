// Fill out your copyright notice in the Description page of Project Settings.

// ****************************************************
// ���[�O���C�N�̃}�X�Ɋ֌W����Enum Class�̃t�@�C��
// ****************************************************

#pragma once

#include "CoreMinimal.h"
#include "E_Tile.generated.h"


// �}�X�̎�ނɊւ���EnumClass
UENUM(BlueprintType)
enum class E_TILE_TYPE : uint8
{
	BATTLE,				// �o�g���}�X
	ITEM,				// �A�C�e���}�X
	HEAL,				// �񕜃}�X
	TILE_TYPE_NUM,		// �}�X�̎�ނ̐�	
	NONE				// ��ޖ��ݒ�̏��
};


