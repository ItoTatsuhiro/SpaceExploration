#pragma once

#include "CoreMinimal.h"
#include "CPPCalcInfo.generated.h"


USTRUCT(BlueprintType)
struct FStatus {

	GENERATED_BODY()

	float HP;
	float AttackPower;
	float DefencePower;
	float Speed;
	enum EElement;
};