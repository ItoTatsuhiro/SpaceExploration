#pragma once

#include "CoreMinimal.h"
#include "tsutsumi/Element.h"
#include "EnemyData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyData
{
	GENERATED_BODY()

	//シーン切り替え時に保持するエネミーデータ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EElement EnemyElement;
};
