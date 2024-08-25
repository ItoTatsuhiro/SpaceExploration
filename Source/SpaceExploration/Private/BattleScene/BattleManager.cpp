// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleScene/BattleManager.h"
#include "Character/CharacterBase.h"
#include "Character/EnemyBase.h"
#include "Character/PlayerCharacter.h"
#include "LevelGroup/LevelInterface.h"
#include "BattleScene/E_BattleSEQ.h"
#include "Kismet/KismetSystemLibrary.h"

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
	//攻撃順初期化
	attack_order.clear();
	//attack_orderの始めにバトル前の準備シーンを設定
	attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_STANDBY));

	//バトル順初期化
	seqindex = 0;
}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//現在実行中のシーケンス
	NowBattleSeq = attack_order[seqindex];

	//バトルシーケンスそれぞれのシーケンス実行
	switch (NowBattleSeq) {
	//バトルスタンバイシーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_STANDBY):
		
	break;
	//プレイヤー攻撃シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK):
		player->StartAttackAction();
	break;
	//プレイヤー攻撃を受けるシーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACKRECEIVE):
		player->TakeDamage(playerdamage);
	break;
	//エネミー攻撃シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACK):
		enemy->StartAttackAction();
	break;
	//エネミー攻撃を受けるシーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE):
		enemy->TakeDamage(enemydamage);
	break;
	//バトルリザルト画面シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_RESULT):
		
	break;
	//バトル終了シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_END):
		levelinterface->LoadLevel(NextLevel);
	break;
	}
}

void ABattleManager::ButtleInit(const  FStatus& player_status, const int& playerelement, const FStatus& enemy_status, const int& enemyelement)
{
	//順番決め用一時変数に挿入
	//プレイヤー
	player_.hp_ = player_status.HP;
	player_.speed_ = player_status.Speed;
	player_.defense_ = player_status.DefencePower;
	player_.attack_ = player_status.AttackPower;
	player_.type_ = playerelement;

	//敵
	enemy_.hp_ = enemy_status.HP;
	enemy_.speed_ = enemy_status.Speed;
	enemy_.defense_ = enemy_status.DefencePower;
	enemy_.attack_ = enemy_status.AttackPower;
	enemy_.type_ = enemyelement;

	//攻撃タイミング設定
	attack_timing_ = player_status.Speed + enemy_status.Speed;

	//順番決め
	BattleTurn();
}

void ABattleManager::BattleTurn()
{
	float playerhp = player_.hp_, enemyhp = enemy_.hp_;

	//プレイヤーか敵のどちらかの体力が0になるまで
	while (playerhp > 0.0f && enemyhp > 0.0f) {
		player_.attack_count_ += player_.speed_;
		enemy_.attack_count_ += enemy_.speed_;

		//プレイヤーの攻撃
		if (player_.attack_count_ >= attack_timing_) {
			player_.attack_count_ = 0.0f;
			//ダメージの計算
			enemydamage = DamageMath(player_.attack_, player_.type_, enemy_.defense_, enemy_.type_);
			enemyhp -= enemydamage;
			//順番を設定
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK));
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE));
		}
		//敵の攻撃
		else if (enemy_.attack_count_ >= attack_timing_) {
			enemy_.attack_count_ = 0.0f;
			//ダメージ計算
			playerdamage = DamageMath(enemy_.attack_, enemy_.type_, player_.defense_, player_.type_);
			playerhp -= playerdamage;
			//順番を設定
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACK));
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACKRECEIVE));
		}
	}
	//バトル終了シーケンス設定
	attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_RESULT));
	attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_END));
}

float ABattleManager::DamageMath(const float& A_atk, const int& A_type, const float& D_def, const int& D_type){
	//タイプ相性補正値の設定
	float type_corr = 0.0;
	//攻撃側の属性相性判定用値
	int a_type_corr = (A_type + 1);

	//ダメージ
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

void ABattleManager::ConvertVectorToActor() {
	//TArrayをクリア
	ConvertArray.Empty();

	//vectorをTArrayにコピー
	for (auto element : attack_order) {
		ConvertArray.Add(element);
	}
}

