// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"

struct FStatus;

enum class CHARACTER {
	player,
	enemy,
};

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
//---------------------------------------------------------------------------------------------
//�o�g���̃^�[�����ߊ֌W

	//����
	enum TYPE {
		fire,
		water,
		wind
	};
	//������������p
	int type_corr_[5] = { wind, fire, water, wind, fire };
	//���������̃_���[�W�␳�l
	//good_ = 2.0
	//bad_ = 0.5
	//none_ = 1.0
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
	
	//�s�������߂�l
	float attack_timing_ = 10;
	//���Ԃ�����z��
	std::vector<CHARACTER> attack_order;

//---------------------------------------------------------------------------------------------

public:
	//�o�g���V�[���ɓ��������̏������֐�
	//�����P�F�v���C���[�̃X�e�[�^�X�f�[�^
	//�����Q�F�v���C���[�̑���
	//�����R�F�G�̃X�e�[�^�X�f�[�^
	//�����S�F�G�̑���
	UFUNCTION(BlueprintCallable)
	void ButtleInit(const  FStatus& player, const int& playerelement, const FStatus& enemy, const int& enemyelement);

	//�U���̏��Ԃ����߂�
	void BattleTurn();

	//�^�[���擾
	std::vector<CHARACTER> GetButtleTurn()const { return attack_order; };

	//�_���[�W�v�Z
	//�����P�F�v���C���[�̍U����
	//�����Q�F�v���C���[�̑���
	//�����R�F�G�̖h���
	//�����S�F�G�̑���
	float DamageMath(const float& A_atk, const int& A_type, const float& D_def, const int& D_type);
};
