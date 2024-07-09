// Fill out your copyright notice in the Description page of Project Settings.

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

	//�v���C���[
	player_.hp_ = 30.0;
	player_.speed_ = 5.0;
	player_.defense_ = 15.0;
	player_.attack_ = 20.0;
	player_.type_ = 0;

	//�G
	enemy_.hp_ = 20.0;
	enemy_.speed_ = 3.0;
	enemy_.defense_ = 15.0;
	enemy_.attack_ = 20.0;
	enemy_.type_ = 0;

	//���Ԍ���
	BattleTurn();

	/*for (int i = 0; i < attack_order.size(); i++) {
		if (attack_order[i] == CHARACTER::player) {
			UKismetSystemLibrary::PrintString(this, "player attack  ", true, true, FColor::Cyan, 60.f, TEXT("None"));
		}
		else if (attack_order[i] == CHARACTER::enemy) {
			UKismetSystemLibrary::PrintString(this, "enemy attack", true, true, FColor::Cyan, 60.f, TEXT("None"));
		}
	}*/

	player_.hp_ -= 10.0;
	FString playerhp = FString::SanitizeFloat(player_.hp_);;

	UKismetSystemLibrary::PrintString(this, playerhp, true, true, FColor::Cyan, 60.f, TEXT("None"));

}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattleManager::BattleTurn()
{
	float playerhp = player_.hp_, enemyhp = enemy_.hp_;

	//�v���C���[���G�̂ǂ��炩�̗̑͂�0�ɂȂ�܂�
	while (playerhp > 0.0f && enemyhp > 0.0f) {
		player_.attack_count_ += player_.speed_;
		enemy_.attack_count_ += enemy_.speed_;

		//�v���C���[�̍U��
		if (player_.attack_count_ >= attack_speed_) {
			player_.attack_count_ = 0.0f;
			//�_���[�W�̌v�Z
			enemyhp -= DamageMath(player_.attack_, player_.type_, enemy_.defense_, enemy_.type_);
			attack_order.emplace_back(CHARACTER::player);
		}
		//�G�̍U��
		else if (enemy_.attack_count_ >= attack_speed_) {
			enemy_.attack_count_ = 0.0f;
			//�_���[�W�v�Z
			playerhp -= DamageMath(enemy_.attack_, enemy_.type_, player_.defense_, player_.type_);
			attack_order.emplace_back(CHARACTER::enemy);
		}
	}
}

float ABattleManager::DamageMath(float A_atk, int A_type, float D_def, int D_type){
	//�^�C�v�����␳�l�̐ݒ�
	float type_corr = 0.0;

	int a_type_corr = (A_type + 1);

	if (D_type == type_corr_[a_type_corr]) {
		type_corr = none_;
	}
	else if (D_type == type_corr_[a_type_corr - 1]) {
		type_corr = good_;
	}
	else if (D_type == type_corr_[a_type_corr + 1]) {
		type_corr = bad_;
	}

	//�_���[�W�v�Z
	int damage = (A_atk / D_def) * type_corr;

	return damage;
}



