#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Status.generated.h"

USTRUCT(BlueprintType)
struct FStatus : public FTableRowBase
{

    GENERATED_BODY()

    FStatus() 
    {
        HP = 1.f;
        MaxHp = 1.f;
        AttackPower = 1.f;
        DefencePower = 1.f;
        Speed = 1.f;
        PlayerLevel = 1;
        PlayerName = "No Name";
        Exp = 0;
    }

    FStatus(float _HP, float _MaxHp, float _AttackPower, float _DefencePower, float _Speed, int _PlayerLevel, FString _PlayerName, int _Exp) 
    {
        HP = _HP;
        MaxHp = _MaxHp;
        AttackPower = _AttackPower;
        DefencePower = _DefencePower;
        Speed = _Speed;
        PlayerLevel = _PlayerLevel;
        PlayerName = _PlayerName;
        Exp = _Exp;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefencePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Speed;

    UPROPERTY(EditAnywhere , BlueprintReadWrite)
    int PlayerLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Exp;

    // オーバーロードされた代入演算子
    FStatus& operator=(const FStatus& status) {
        if (this != &status) {
            HP = status.HP;
            MaxHp = status.MaxHp;
            AttackPower = status.AttackPower;
            DefencePower = status.DefencePower;
            Speed = status.Speed;
            PlayerName = status.PlayerName;
            PlayerLevel = status.PlayerLevel;
            Exp = status.Exp;
        }
        return *this;
    }
};

