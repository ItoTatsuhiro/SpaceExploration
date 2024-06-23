// Fill out your copyright notice in the Description page of Project Settings.


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
	player_.defense_ = 5.0;
	player_.attack_ = 15.0;

	//�G
	enemy_.hp_ = 20.0;
	enemy_.speed_ = 3.0;
	enemy_.defense_ = 2.0;
	enemy_.attack_ = 10.0;

	//���Ԍ���
	BattleTurn();
}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattleManager::BattleTurn()
{
	//�v���C���[���G�̂ǂ��炩�̗̑͂�0�ɂȂ�܂�
	while (player_hp_ <= 0.0f || enemy_hp_ <= 0.0f) {
		player_attack_count_ += player_speed_;
		enemy_attack_count_ += enemy_speed_;

		if (player_attack_count_ >= attack_speed_) {
			player_attack_count_ = 0.0f;

			attack_order.emplace_back(player_);
		}
		else if (enemy_attack_count_ >= attack_speed_) {
			enemy_attack_count_ = 0.0f;

			attack_order.emplace_back(enemy_);
		}
	}

}

