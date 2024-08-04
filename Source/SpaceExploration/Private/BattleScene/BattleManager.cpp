// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BattleScene/BattleManager.h"

// Sets default values
ABattleManager::ABattleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattleManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattleManager::ButtleInit(const  FStatus& player, const int& playerelement, const FStatus& enemy, const int& enemyelement)
{
	//���Ԍ��ߗp�ꎞ�ϐ��ɑ}��
	
	//�v���C���[
	player_.hp_ = player.HP;
	player_.speed_ = player.Speed;
	player_.defense_ = player.DefencePower;
	player_.attack_ = player.AttackPower;
	player_.type_ = playerelement;

	//�G
	enemy_.hp_ = enemy.HP;
	enemy_.speed_ = enemy.Speed;
	enemy_.defense_ = enemy.DefencePower;
	enemy_.attack_ = enemy.AttackPower;
	enemy_.type_ = enemyelement;

	//�U���^�C�~���O�ݒ�
	attack_timing_ = player.Speed + enemy.Speed;

	//���Ԍ���
	BattleTurn();
}

void ABattleManager::BattleTurn()
{
	float playerhp = player_.hp_, enemyhp = enemy_.hp_;

	//�v���C���[���G�̂ǂ��炩�̗̑͂�0�ɂȂ�܂�
	while (playerhp > 0.0f && enemyhp > 0.0f) {
		player_.attack_count_ += player_.speed_;
		enemy_.attack_count_ += enemy_.speed_;

		//�v���C���[�̍U��
		if (player_.attack_count_ >= attack_timing_) {
			player_.attack_count_ = 0.0f;
			//�_���[�W�̌v�Z
			enemyhp -= DamageMath(player_.attack_, player_.type_, enemy_.defense_, enemy_.type_);
			attack_order.emplace_back(CHARACTER::player);
		}
		//�G�̍U��
		else if (enemy_.attack_count_ >= attack_timing_) {
			enemy_.attack_count_ = 0.0f;
			//�_���[�W�v�Z
			playerhp -= DamageMath(enemy_.attack_, enemy_.type_, player_.defense_, player_.type_);
			attack_order.emplace_back(CHARACTER::enemy);
		}
	}
}

float ABattleManager::DamageMath(const float& A_atk, const int& A_type, const float& D_def, const int& D_type){
	//�^�C�v�����␳�l�̐ݒ�
	float type_corr = 0.0;
	//�U�����̑�����������p�l
	int a_type_corr = (A_type + 1);

	//�_���[�W
	int damage = 0;

	if (D_type == static_cast<int>(type_corr_[a_type_corr])) {
		damage = (A_atk / D_def) * none_;
	}
	else if (D_type == static_cast<int>(type_corr_[a_type_corr - 1])) {
		damage = (A_atk / D_def) * good_;
	}
	else if (D_type == static_cast<int>(type_corr_[a_type_corr + 1])) {
		damage = (A_atk / D_def) * bad_;
	}

	return damage;
}



