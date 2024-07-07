// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"

UCLASS()
class SPACEEXPLORATION_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//����
	enum TYPE {
		fire,
		water,
		wind
	};
	
	//������������p
	int type_corr_[5] = { wind, fire, water, wind, fire };

	//���������̃_���[�W�␳�l
	const float good_ = 2.0, bad_ = 0.5, none_ = 1.0;

	struct character {
		float hp_ = 0;
		float speed_ = 0.0;
		float defense_ = 0.0;
		float attack_ = 0.0;
		//�����@0���΁@1�����@2����
		//���������@�΁����@�������@������
		int type_ = 0;

		float attack_count_ = 0.0;
	};
	character player_;
	character enemy_;

	enum class CHARACTER {
		player,
		enemy,
	};

	//�s�������߂�l
	float attack_speed_ = 10;
	//���Ԃ�����z��
	std::vector<CHARACTER> attack_order;

public:
	//�U���̏��Ԃ����߂�
	void BattleTurn();

	//�_���[�W�v�Z
	//�����P�E�U�����̍U����
	//�����Q�E�U�����̑���
	//�����R�E�h�䑤�̖h���
	//�����S�E�h�䑤�̑���
	float DamageMath(float A_atk, int A_type, float D_def, int D_type);
};
