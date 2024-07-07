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
	struct character {
		float hp_ = 0;
		float speed_ = 0.0;
		float defense_ = 0.0;
		float attack_ = 0.0;
		//�����@0���΁@1�����@2����
		int _type_ = 0;

		float attack_count_ = 0.0;
	};

	character player_;
	character enemy_;

	//�̗�
	float player_hp_ = 30, enemy_hp_ = 20;
	//�v���C���[�ƃG�l�~�[�̑���
	float player_speed_ = 5.0f, enemy_speed_ = 3.0f;
	//

	//�U�����Ԍ��߂̃J�E���g
	float player_attack_count_ = 0.0f, enemy_attack_count_ = 0.0f;
	//�s�������߂�l
	float attack_speed_ = 10;
	//���Ԃ�����z��
	std::vector<character> attack_order;

	//�_���[�W�v�Z
	void DamageMath();
	//�U���̏��Ԃ����߂�
	void BattleTurn();
};
