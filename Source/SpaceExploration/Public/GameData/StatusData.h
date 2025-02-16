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

	// レベル
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Level;

	// 基本最大HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseMaxHp;

	// 基本攻撃力
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseAttack;

	// 基本防御力
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDefence;

	// 基本、攻撃速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseSpeed;

	// 最大HPのランダムで増える幅
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RandomMaxHp;

	// 攻撃力のランダムで増える幅
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RandomAttack;

	// 防御力のランダムで増える幅
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RandomDefence;

	// 攻撃速度のランダムで増える幅
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RandomSpeed;

	// 経験値
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Exp;
};