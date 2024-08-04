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

	// マスの数をカウント
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


// ランダムなマスの種類を返す関数
E_TILE_TYPE AGalaxyRandomSelect::RandomTileSelect() {

	//// マスの種類に対して比重の配列に入っている数が異なるとき警告を出す
	//if (selectWeight_.Num() < tileTypeNum_) {
	//	UE_LOG(LogTemp, Log, TEXT("比重の数が足りません\n") );
	//}

	// 比重の合計
	float totalWeight = 0.0f;
	// 累積の比重の配列
	TArray<float> cumulativeWeight;

	for (int i = 0; i < selectWeight_.Num(); ++i) {
		// 比重の合計に加算 
		totalWeight += selectWeight_[i];
		// 累積の比重の配列に追加
		cumulativeWeight.Emplace(totalWeight);
	}

	// ランダムな値を決定
	float selectNum = FMath::FRandRange(0.0f, totalWeight);

	for (int i = 0; i < cumulativeWeight.Num(); ++i) {
		if (selectNum < cumulativeWeight[i]) {
			return static_cast<E_TILE_TYPE>(i);
		}
	}

	// デフォルト値を返す
	return E_TILE_TYPE::NONE;

	//int selectTile = std::rand() % tileTypeNum_;

	//return static_cast<E_TILE_TYPE>(selectTile);
}

// 引数の種類のマスを、ランダムに選択したマスに入れるための関数
void AGalaxyRandomSelect::RandomInsertTileType(E_TILE_TYPE insertTile) {

	// マスの数以内の、ランダムな数値を決定
	int randomNum = FMath::RandRange(0, tileCount_ - 1);
	// 決定したランダムな数値まで確認するうちの、何番目の要素まで確認を行ったか置いておくための変数
	// ※配列の要素の数が一定でないため
	int checkNum = 0;

	// 配列の頭から順番に、数値をずらしていく処理
	for (int i = 0; i < tileArray_.Num(); ++i) {
		checkNum += tileNumHolizontal_[i];

		// 現在確認している要素までの配列の大きさの累計が、ランダムに決定した数よりも大きくなったとき処理
		if (checkNum > randomNum) {
			// 配列の何番目がランダムな数値番目の要素に該当するか確認
			int row = randomNum - (checkNum - tileNumHolizontal_[i]);
			// マスを入れる箇所の要素番号を変数として決定
			// CheckShiftArray関数により、空でない場合はずらす
			 FVector2D newIndex = CheckShiftArray(FVector2D(row, i));

			// 使用しやすいようにx, y毎にint型の変数を用意
			int y = static_cast<int>(newIndex.Y);
			int x = static_cast<int>(newIndex.X);

			// 配列の範囲内かどうかを確認
			if (y >= 0 && y < tileArray_.Num() && x >= 0 && x < tileArray_[y].Num()) {
				// 配列の中身が空の状態の時のみ入れる
				if (tileArray_[y][x] == E_TILE_TYPE::NONE) {
					tileArray_[y][x] = insertTile;
					break;
				}
			}
			else {
				// 配列外となってしまった場合はログを表示するようにする
				UE_LOG(LogTemp, Error, TEXT("Index out of range: x=%d, y=%d"), x, y);
			}
		}
	}
}


// 配列内のマスの種類が入っているか確認し、入っていれば添え字をずらす関数
// ずらした先にさらに空のマスがあるか確認し、マスがある位置までずらす
FVector2D AGalaxyRandomSelect::CheckShiftArray(FVector2D before) {

	FVector2D newArrayNum = before;


	while (true) {
		// インデックスの範囲を確認
		if (newArrayNum.Y >= tileArray_.Num() || newArrayNum.X >= tileNumHolizontal_[static_cast<int>(newArrayNum.Y)]) {
			// 配列範囲外のインデックスが検出された場合の処理
			UE_LOG(LogTemp, Error, TEXT("Index out of bounds: X = %d, Y = %d"), static_cast<int>(newArrayNum.X), static_cast<int>(newArrayNum.Y));

			// 範囲外の時は00にする
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

			// `newArrayNum.Y` が配列の範囲内であるか確認
			if (newArrayNum.Y >= tileArray_.Num()) {
				newArrayNum.Y = 0; // または他のエラー処理
			}
		}
	}

	return newArrayNum;
}


// Called when the game starts or when spawned
void AGalaxyRandomSelect::BeginPlay()
{
  
	Super::BeginPlay();

	////forループで一つのステージの惑星を二次元配列に格納しています。[0][0]と[2][0]には何も格納されていません
	//for (int i = 0; i <= 2; i++) {
	//	for (int j = 0; j <= 2; j++) {
	//		if (i == 0 && j == 0) {}
	//		else if (i == 2 && j == 0) {}
	//		else {
	//			//乱数から惑星をPlanetLine配列に格納しています
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


// マスの配列を作成する関数
void AGalaxyRandomSelect::MakeTileArray() {
	// マスの数分、配列の枠を生成
	for (int v = 0; v < tileNumHolizontal_.Num(); ++v) {
		// 二重配列に入れる用の仮の配列
		TArray<E_TILE_TYPE> tempTileArray;

		// 横方向のマスの数だけランダムにマスの種類を決定
		for (int h = 0; h < tileNumHolizontal_[v]; ++h) {
			tempTileArray.Emplace(E_TILE_TYPE::NONE);
		}

		// 二重配列に入れる
		tileArray_.Emplace(tempTileArray);
	}


	// 最低限生成する必要のあるマスの数の合計
	int totalMinSelectCount = 0;
	for (int i = 0; i < minSelectNum_.Num(); ++i) {
		totalMinSelectCount += minSelectNum_[i];
	}

	// 最低限生成する必要のあるマスの合計分回す
	for (int i = 0; i < totalMinSelectCount; ++i) {
		// マスの種類数分確認を行う
		for (int j = 0; j < minSelectNum_.Num(); ++j) {
			// 現在のマスの種類までに生成する最低限の数
			int tileCreateCount = 0;

			// 現在時点での生成するマスの種類の確認
			for (int k = 0; k < j; ++k) {
				tileCreateCount += minSelectNum_[k];
			}

			// 現在作成するべき数よりも生成数が少ないとき
			if (i < tileCreateCount) {
				// 現在生成するマス
				E_TILE_TYPE createTile = static_cast<E_TILE_TYPE>(j);

				
				RandomInsertTileType(createTile);
				break;
			}
		}
	}

	// 該当する箇所にランダムに選択したマスの種類をセット
	for (int v = 0; v < tileNumHolizontal_.Num(); ++v) {
		for (int h = 0; h < tileNumHolizontal_[v]; ++h) {
			// 既にマスが存在している場合は処理しない
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


	UE_LOG(LogTemp, Log, TEXT("生成完了！"));
}