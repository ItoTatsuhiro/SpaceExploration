// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StatusData.generated.h"

/**
 *　敵や武器のステータスを設定を行うためのデータテーブル用の構造体
 */
USTRUCT(BlueprintType)
struct FStatusData : public FTableRowBase
{
	GENERATED_BODY()

	FStatusData()
	{
		Level = 1;
		BaseMaxHp = 1.f;
		BaseAttack = 1.f;
		BaseDefence = 1.f;
		BaseSpeed = 1.f;
		RandomMaxHp = 1.f;
		RandomAttack = 1.f;
		RandomDefence = 1.f;
		RandomSpeed = 1.f;
		Exp = 1;
	}

	// レベル
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	// 基本最大HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMaxHp;

	// 基本攻撃力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttack;

	// 基本防御力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDefence;

	// 基本、攻撃速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSpeed;

	// 最大HPのランダムで増える幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RandomMaxHp;

	// 攻撃力のランダムで増える幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RandomAttack;

	// 防御力のランダムで増える幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RandomDefence;

	// 攻撃速度のランダムで増える幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RandomSpeed;

	// 経験値
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Exp;
};