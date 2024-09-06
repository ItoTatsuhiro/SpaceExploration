// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleScene/BattleManager.h"
#include "Character/CharacterBase.h"
#include "Character/EnemyBase.h"
#include "Character/PlayerCharacter.h"
#include "LevelGroup/LevelInterface.h"
#include "BattleScene/E_BattleSEQ.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABattleManager::ABattleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ABattleManager::~ABattleManager()
{

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

	//現在実行中のシーケンス
	NowBattleSeq = attack_order[seqindex];

	//バトルシーケンスそれぞれのシーケンス実行
	switch (NowBattleSeq) {
	//バトルスタンバイシーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_STANDBY):
		UKismetSystemLibrary::PrintString(this, "~BATTLE_STANDBY~", true, true, FColor::Cyan, 2.f, TEXT("None"));
		//バトルシーンのカメラに切り替え
		playercontroller->SetViewTargetWithBlend(BattleSceneCamera);
	break;
	//プレイヤー攻撃シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK):
		player->StartAttackAction();
		//プレイヤーのカメラに切り替え
		playercontroller->SetViewTargetWithBlend(player->GetBattleCameraComponent()->GetChildActor());
	break;
	//プレイヤー攻撃を受けるシーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACKRECEIVE):
		player->TakeDamage(playerdamage);
		//プレイヤーのカメラに切り替え
		playercontroller->SetViewTargetWithBlend(player->GetBattleCameraComponent()->GetChildActor());
	break;
	//エネミー攻撃シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACK):
		enemy->StartAttackAction();
		//敵カメラに切り替え
		playercontroller->SetViewTargetWithBlend(enemy->GetBattleCameraComponent()->GetChildActor());
	break;
	//エネミー攻撃を受けるシーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE):
		enemy->TakeDamage(enemydamage);
		//敵カメラに切り替え
		playercontroller->SetViewTargetWithBlend(enemy->GetBattleCameraComponent()->GetChildActor());
	break;
	//バトルリザルト画面シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_RESULT):
		UKismetSystemLibrary::PrintString(this, "~BATTLE_RESULT~", true, true, FColor::Cyan, 2.f, TEXT("None"));
		//バトルシーンのカメラに切り替え
		playercontroller->SetViewTargetWithBlend(BattleSceneCamera);
	break;
	//バトル終了シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_END):
		
	break;
	}
}

void ABattleManager::ButtleInit()
{
	//攻撃順初期化
	attack_order.clear();
	//attack_orderの始めにバトル前の準備シーンを設定
	attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_STANDBY));

	//現在のバトル順初期化
	seqindex = 0;

	//順番決め用一時変数に挿入
	//プレイヤー
	playerstatus_ = player->GetCharacterStatus();
	provplayerstatus_.hp_ = playerstatus_.HP;
	provplayerstatus_.type_ = static_cast<uint8>(player->GetEquippedWeapon()->GetWeaponElement());;
	
	//敵
	enemystatus_ = levelinterface->GetterBattleEnemy()->GetCharacterStatus();
	provenemystatus_.hp_ = enemystatus_.HP;
	provenemystatus_.type_ = levelinterface->GetterBattleEnemyElement();

	//攻撃タイミング設定
	attack_timing_ = playerstatus_.Speed + enemystatus_.Speed;

	//順番決め
	BattleTurn();
}

void ABattleManager::BattleTurn()
{
	//ダメージの計算
	enemydamage = DamageMath(playerstatus_.AttackPower, provplayerstatus_.type_, enemystatus_.DefencePower, provenemystatus_.type_);
	playerdamage = DamageMath(enemystatus_.AttackPower, provenemystatus_.type_, playerstatus_.DefencePower, provplayerstatus_.type_);

	//プレイヤーか敵のどちらかの体力が0になるまで
	while (playerstatus_.HP > 0.0f && enemystatus_.HP > 0.0f) {
		provplayerstatus_.attack_count_ += playerstatus_.Speed;
		provenemystatus_.attack_count_ += enemystatus_.Speed;

		//プレイヤーの攻撃
		if (provplayerstatus_.attack_count_ >= attack_timing_) {
			provplayerstatus_.attack_count_ = 0.0f;
			
			enemystatus_.HP -= enemydamage;

			//順番を設定
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK));
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE));
		}

		//敵の攻撃
		if (provenemystatus_.attack_count_ >= attack_timing_) {
			provenemystatus_.attack_count_ = 0.0f;

			playerstatus_.HP -= playerdamage;

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

