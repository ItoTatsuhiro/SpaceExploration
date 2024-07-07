#pragma once

#include "CoreMinimal.h"
#include "Status.generated.h"

USTRUCT(BlueprintType)
struct FStatus {

    GENERATED_BODY()

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

    UPROPERTY(EditAnywhere)
    FString PlayerName;


    

   
    



    // �I�[�o�[���[�h���ꂽ������Z�q
    FStatus& operator=(const FStatus& status) {
        if (this != &status) {
            HP = status.HP;
            MaxHp = status.MaxHp;
            AttackPower = status.AttackPower;
            DefencePower = status.DefencePower;
            Speed = status.Speed;
        }
        return *this;
    }
};

// �O���[�o����FStatus�ϐ�
FStatus PlayerStatus;
FStatus Copy = PlayerStatus;
