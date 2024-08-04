// Fill out your copyright notice in the Description page of Project Settings.


#include "tsutsumi/GalaxyRandomSelect.h"
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"




// Sets default values
AGalaxyRandomSelect::AGalaxyRandomSelect()
	: tileNumHolizontal_( { 1, 2, 3, 2 } ), tileTypeNum_(static_cast<int>(E_TILE_TYPE::TILE_TYPE_NUM))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �}�X�̐����J�E���g
	for (int i = 0; i < tileNumHolizontal_.Num(); ++i) {
		tileCount_ += tileNumHolizontal_[i];
	}

	if (minSelectNum_.Num() != tileTypeNum_) {

		for (int i = minSelectNum_.Num(); i < tileTypeNum_; ++i) {
			minSelectNum_.Emplace(1);
		}

	}


	MakeTileArray();
}


// �����_���ȃ}�X�̎�ނ�Ԃ��֐�
E_TILE_TYPE AGalaxyRandomSelect::RandomTileSelect() {

	//// �}�X�̎�ނɑ΂��Ĕ�d�̔z��ɓ����Ă��鐔���قȂ�Ƃ��x�����o��
	//if (selectWeight_.Num() < tileTypeNum_) {
	//	UE_LOG(LogTemp, Log, TEXT("��d�̐�������܂���\n") );
	//}

	// ��d�̍��v
	float totalWeight = 0.0f;
	// �ݐς̔�d�̔z��
	TArray<float> cumulativeWeight;

	for (int i = 0; i < selectWeight_.Num(); ++i) {
		// ��d�̍��v�ɉ��Z 
		totalWeight += selectWeight_[i];
		// �ݐς̔�d�̔z��ɒǉ�
		cumulativeWeight.Emplace(totalWeight);
	}

	// �����_���Ȓl������
	float selectNum = FMath::FRandRange(0.0f, totalWeight);

	for (int i = 0; i < cumulativeWeight.Num(); ++i) {
		if (selectNum < cumulativeWeight[i]) {
			return static_cast<E_TILE_TYPE>(i);
		}
	}

	// �f�t�H���g�l��Ԃ�
	return E_TILE_TYPE::NONE;

	//int selectTile = std::rand() % tileTypeNum_;

	//return static_cast<E_TILE_TYPE>(selectTile);
}

// �����̎�ނ̃}�X���A�����_���ɑI�������}�X�ɓ���邽�߂̊֐�
void AGalaxyRandomSelect::RandomInsertTileType(E_TILE_TYPE insertTile) {

	// �}�X�̐��ȓ��́A�����_���Ȑ��l������
	int randomNum = FMath::RandRange(0, tileCount_ - 1);
	// ���肵�������_���Ȑ��l�܂Ŋm�F���邤���́A���Ԗڂ̗v�f�܂Ŋm�F���s�������u���Ă������߂̕ϐ�
	// ���z��̗v�f�̐������łȂ�����
	int checkNum = 0;

	// �z��̓����珇�ԂɁA���l�����炵�Ă�������
	for (int i = 0; i < tileArray_.Num(); ++i) {
		checkNum += tileNumHolizontal_[i];

		// ���݊m�F���Ă���v�f�܂ł̔z��̑傫���̗݌v���A�����_���Ɍ��肵���������傫���Ȃ����Ƃ�����
		if (checkNum > randomNum) {
			// �z��̉��Ԗڂ������_���Ȑ��l�Ԗڂ̗v�f�ɊY�����邩�m�F
			int row = randomNum - (checkNum - tileNumHolizontal_[i]);
			// �}�X������ӏ��̗v�f�ԍ���ϐ��Ƃ��Č���
			// CheckShiftArray�֐��ɂ��A��łȂ��ꍇ�͂��炷
			 FVector2D newIndex = CheckShiftArray(FVector2D(row, i));

			// �g�p���₷���悤��x, y����int�^�̕ϐ���p��
			int y = static_cast<int>(newIndex.Y);
			int x = static_cast<int>(newIndex.X);

			// �z��͈͓̔����ǂ������m�F
			if (y >= 0 && y < tileArray_.Num() && x >= 0 && x < tileArray_[y].Num()) {
				// �z��̒��g����̏�Ԃ̎��̂ݓ����
				if (tileArray_[y][x] == E_TILE_TYPE::NONE) {
					tileArray_[y][x] = insertTile;
					break;
				}
			}
			else {
				// �z��O�ƂȂ��Ă��܂����ꍇ�̓��O��\������悤�ɂ���
				UE_LOG(LogTemp, Error, TEXT("Index out of range: x=%d, y=%d"), x, y);
			}
		}
	}
}


// �z����̃}�X�̎�ނ������Ă��邩�m�F���A�����Ă���ΓY���������炷�֐�
// ���炵����ɂ���ɋ�̃}�X�����邩�m�F���A�}�X������ʒu�܂ł��炷
FVector2D AGalaxyRandomSelect::CheckShiftArray(FVector2D before) {

	FVector2D newArrayNum = before;


	while (true) {
		// �C���f�b�N�X�͈̔͂��m�F
		if (newArrayNum.Y >= tileArray_.Num() || newArrayNum.X >= tileNumHolizontal_[static_cast<int>(newArrayNum.Y)]) {
			// �z��͈͊O�̃C���f�b�N�X�����o���ꂽ�ꍇ�̏���
			UE_LOG(LogTemp, Error, TEXT("Index out of bounds: X = %d, Y = %d"), static_cast<int>(newArrayNum.X), static_cast<int>(newArrayNum.Y));

			// �͈͊O�̎���00�ɂ���
			newArrayNum = { 0, 0 };

		}

		UE_LOG(LogTemp, Log, TEXT("Checking tileArray_[%d][%d]"), static_cast<int>(newArrayNum.Y), static_cast<int>(newArrayNum.X));

		if (tileArray_[static_cast<int>(newArrayNum.Y)][static_cast<int>(newArrayNum.X)] == E_TILE_TYPE::NONE) {
			break;
		}

		++newArrayNum.X;

		if (newArrayNum.X >= tileNumHolizontal_[static_cast<int>(newArrayNum.Y)]) {
			newArrayNum.X = 0;
			newArrayNum.Y = static_cast<int>(newArrayNum.Y) + 1;

			// `newArrayNum.Y` ���z��͈͓̔��ł��邩�m�F
			if (newArrayNum.Y >= tileArray_.Num()) {
				newArrayNum.Y = 0; // �܂��͑��̃G���[����
			}
		}
	}

	return newArrayNum;
}


// Called when the game starts or when spawned
void AGalaxyRandomSelect::BeginPlay()
{
  
	Super::BeginPlay();

	////for���[�v�ň�̃X�e�[�W�̘f����񎟌��z��Ɋi�[���Ă��܂��B[0][0]��[2][0]�ɂ͉����i�[����Ă��܂���
	//for (int i = 0; i <= 2; i++) {
	//	for (int j = 0; j <= 2; j++) {
	//		if (i == 0 && j == 0) {}
	//		else if (i == 2 && j == 0) {}
	//		else {
	//			//��������f����PlanetLine�z��Ɋi�[���Ă��܂�
	//			int random = RandomProgram();
	//			if (random == static_cast<int>(E_TILE_TYPE::BATTLE)) {
	//				PlanetLine[i][j] = E_TILE_TYPE::BATTLE;
	//			}
	//			else if (random == static_cast<int>(E_TILE_TYPE::BATTLE)) {
	//			    PlanetLine[i][j] = E_TILE_TYPE::ITEM;
	//			}
	//			else if (random == static_cast<int>(E_TILE_TYPE::HEAL))  { 
	//				PlanetLine[i][j] = E_TILE_TYPE::HEAL;
	//			}
	//		
	//		}
	//	 }
	//}



	for (int v = 0; v < tileNumHolizontal_.Num(); ++v) {


		for (int h = 0; h < tileNumHolizontal_[v]; ++h) {

			UE_LOG(LogTemp, Log, TEXT("x:%d, y:%d, tileType = %d"), v, h, tileArray_[v][h]);

		}


	}

	

}

// Called every frame
void AGalaxyRandomSelect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}


// �}�X�̔z����쐬����֐�
void AGalaxyRandomSelect::MakeTileArray() {
	// �}�X�̐����A�z��̘g�𐶐�
	for (int v = 0; v < tileNumHolizontal_.Num(); ++v) {
		// ��d�z��ɓ����p�̉��̔z��
		TArray<E_TILE_TYPE> tempTileArray;

		// �������̃}�X�̐����������_���Ƀ}�X�̎�ނ�����
		for (int h = 0; h < tileNumHolizontal_[v]; ++h) {
			tempTileArray.Emplace(E_TILE_TYPE::NONE);
		}

		// ��d�z��ɓ����
		tileArray_.Emplace(tempTileArray);
	}


	// �Œ����������K�v�̂���}�X�̐��̍��v
	int totalMinSelectCount = 0;
	for (int i = 0; i < minSelectNum_.Num(); ++i) {
		totalMinSelectCount += minSelectNum_[i];
	}

	// �Œ����������K�v�̂���}�X�̍��v����
	for (int i = 0; i < totalMinSelectCount; ++i) {
		// �}�X�̎�ސ����m�F���s��
		for (int j = 0; j < minSelectNum_.Num(); ++j) {
			// ���݂̃}�X�̎�ނ܂łɐ�������Œ���̐�
			int tileCreateCount = 0;

			// ���ݎ��_�ł̐�������}�X�̎�ނ̊m�F
			for (int k = 0; k < j; ++k) {
				tileCreateCount += minSelectNum_[k];
			}

			// ���ݍ쐬����ׂ������������������Ȃ��Ƃ�
			if (i < tileCreateCount) {
				// ���ݐ�������}�X
				E_TILE_TYPE createTile = static_cast<E_TILE_TYPE>(j);

				
				RandomInsertTileType(createTile);
				break;
			}
		}
	}

	// �Y������ӏ��Ƀ����_���ɑI�������}�X�̎�ނ��Z�b�g
	for (int v = 0; v < tileNumHolizontal_.Num(); ++v) {
		for (int h = 0; h < tileNumHolizontal_[v]; ++h) {
			// ���Ƀ}�X�����݂��Ă���ꍇ�͏������Ȃ�
			if (tileArray_[v][h] != E_TILE_TYPE::NONE) {
				continue;
			}
			tileArray_[v][h] = RandomTileSelect();
		}
	}



	for (int v = 0; v < tileNumHolizontal_.Num(); ++v) {


		for (int h = 0; h < tileNumHolizontal_[v]; ++h) {

			UE_LOG(LogTemp, Log, TEXT("x:%d, y:%d, tileType = %d"), v, h, tileArray_[v][h]);

		}


	}


	UE_LOG(LogTemp, Log, TEXT("���������I"));
}