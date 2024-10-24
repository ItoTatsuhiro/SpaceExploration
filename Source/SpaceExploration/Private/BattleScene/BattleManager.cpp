// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleScene/BattleManager.h"
#include "Character/CharacterBase.h"
#include "Character/EnemyBase.h"
#include "LevelGroup/LevelInterface.h"
#include "Character/PlayerCharacter.h"
#include "MyGameInstance.h"
#include "BattleScene/E_BattleSEQ.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABattleManager::ABattleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	attackparticl = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AttackNiagara"));
	attackparticl->SetupAttachment(RootComponent);

}

ABattleManager::~ABattleManager()
{

}

// Called when the game starts or when spawned
void ABattleManager::BeginPlay()
{
	Super::BeginPlay();

	//カメラ切り替え用PlayerControllerを取得
	playercontroller = UGameplayStatics::GetPlayerController(this, 0);
	//ゲームインスタンス取得
	mygameinstance = Cast<UMyGameInstance>(GetGameInstance());

//プレイヤー情報取得時の例外処理-----------------------------------------------------------------------
	if (!UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0)) {
		UE_LOG(LogClass, Warning, TEXT("error : NO playerstatus\n"));
		//エラー落ちしないように仮のステータスを挿入
		playerstatus_.HP = 30.0f;
		playerstatus_.MaxHp = 30.0f;
		playerstatus_.AttackPower = 50.0f;
		playerstatus_.DefencePower = 20.0f;
		playerstatus_.Speed = 15.0f;
		provplayerstatus_.hp_ = playerstatus_.HP;
		provplayerstatus_.type_ = 1;
	}
	else {
		UE_LOG(LogClass, Log, TEXT("success playerstatus load\n"));
		//プレイヤーの情報取得
		player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0));
		//順番決めよう一時変数挿入
		playerstatus_ = player->GetCharacterStatus();
		provplayerstatus_.hp_ = playerstatus_.HP;
		provplayerstatus_.type_ = static_cast<uint8>(player->GetEquippedWeapon()->GetWeaponElement());
	}

	//エネミー情報取得時の例外処理
	if (!mygameinstance->GetterBattleEnemyStatus()) {
		UE_LOG(LogClass, Warning, TEXT("error : NO enemystatus\n"));
		//エラー落ちしないように仮のステータスを挿入
		enemystatus_.HP = 30.0f;
		enemystatus_.AttackPower = 50.0f;
		enemystatus_.DefencePower = 20.0f;
		enemystatus_.Speed = 10.0f;
		provenemystatus_.hp_ = enemystatus_.HP;
		provenemystatus_.type_ = 0;
	}
	else {
		UE_LOG(LogClass, Log, TEXT("success enemystatus load\n"));
		//エネミーのステータス、属性を取得
		enemy = mygameinstance->GetterBattleEnemyStatus();
		//順番決めよう一時変数挿入
		enemystatus_ = enemy->GetCharacterStatus();
		provenemystatus_.hp_ = enemystatus_.HP;
		provenemystatus_.type_ = mygameinstance->GetterBattleEnemyElement();
	}

	//エネミー属性取得時の例外処理
	if (mygameinstance->GetterBattleEnemyElement() > 2 || mygameinstance->GetterBattleEnemyElement() < 0) {
		UE_LOG(LogClass, Warning, TEXT("error : NO enemytype\n"));
		provenemystatus_.type_ = 0;
	}
	else {
		UE_LOG(LogClass, Log, TEXT("success enemyelement load\n"));
		//エネミーのステータス、属性を取得
		provenemystatus_.type_ = mygameinstance->GetterBattleEnemyElement();
	}
//-----------------------------------------------------------------------------------------------------


	//カメラをバトルシーン全体を見る物に切り替え
	playercontroller->SetViewTargetWithBlend(BattleSceneCamera, 1.0);
	cameras = Camera::battlecamera;
	//プレイヤーのカメラと座標設定
	if (player) {
		UE_LOG(LogClass, Log, TEXT("success playerCamera load\n"));
		//カメラ設定
		PlayerCamera = Cast<AActor>(player->GetBattleCameraComponent());
		//座標設定
		player->SetCharacterLocation(playerpos_actor->GetActorLocation());
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("error : No playerCamera\n"));
		PlayerCamera = BattleSceneCamera;
	}
	//敵のカメラと座標設定
	if (!enemy) {
		UE_LOG(LogClass, Warning, TEXT("error : No enemyCamera\n"));
		EnemyCamera = BattleSceneCamera;
	}
	else {
		UE_LOG(LogClass, Log, TEXT("success enemyCamera load\n"));
		//カメラ設定
		EnemyCamera = Cast<AActor>(enemy->GetBattleCameraComponent());
		//座標設定
		enemy->SetActorLocation(enemypos_actor->GetActorLocation());
	}

	//パーティクル
	attackparticl->SetRelativeLocation(particlattack_->GetActorLocation());
	attackparticl->SetWorldRotation(FRotator3d(0,-90,0));

	//バトル順など初期化
	ButtleInit();
}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//現在実行中のシーケンス
	NowBattleSeq = attack_order[seqindex];

	//テストバトルシーケンスそれぞれのシーケンス実行
	switch (NowBattleSeq) {
		//バトルスタンバイシーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_STANDBY):
		if (cameras != Camera::battlecamera) {
			//バトルシーンのカメラに切り替え
			playercontroller->SetViewTargetWithBlend(BattleSceneCamera, 1.0);
			cameras = Camera::battlecamera;
		}
		
		//8UKismetSystemLibrary::PrintString(this, "~BATTLE_STANDBY~", true, true, FColor::Cyan, 2.f, TEXT("None"));

		_count += DeltaTime;
		//次のターンに進める
		if (_count >= _time) {
			SeqIndexAdd();
			_count = 0.0f;
		}		
		break;
		//プレイヤー攻撃シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK):
		if (cameras != Camera::playercamera) {
			//プレイヤーのカメラに切り替え
			playercontroller->SetViewTargetWithBlend(PlayerCamera, 1.0);
			cameras = Camera::playercamera;
		}
		UKismetSystemLibrary::PrintString(this, "~BATTLE_PLAYERATTACK~", true, true, FColor::Cyan, 2.f, TEXT("None"));

		//プレイヤー攻撃関数
		//player->StartAttackAction();
		//パーティクル再生
		attackparticl->Activate();

		_count += DeltaTime;
		//次のターンに進める
		if (_count >= _time) {
			SeqIndexAdd();
			_count = 0.0f;
		}
		break;
		//プレイヤー攻撃を受けるシーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACKRECEIVE):
		if (cameras != Camera::playercamera) {
			//プレイヤーのカメラに切り替え
			playercontroller->SetViewTargetWithBlend(PlayerCamera, 1.0);
			cameras = Camera::playercamera;
		}
		UKismetSystemLibrary::PrintString(this, "~BATTLE_PLAYERHIT~", true, true, FColor::Cyan, 2.f, TEXT("None"));

		//プレイヤー攻撃を受ける関数
		//player->TakeDamage(playerdamage);

		_count += DeltaTime;
		//次のターンに進める
		if (_count >= _time) {
			SeqIndexAdd();
			_count = 0.0f;
		}

		break;
		//エネミー攻撃シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACK):
		if (cameras != Camera::enemycamera) {
			//敵カメラに切り替え
			playercontroller->SetViewTargetWithBlend(EnemyCamera, 1.0);
			cameras = Camera::enemycamera;
		}
		UKismetSystemLibrary::PrintString(this, "~BATTLE_ENEMYATTACK~", true, true, FColor::Cyan, 2.f, TEXT("None"));

		//敵攻撃関数
		//enemy->StartAttackAction();

		_count += DeltaTime;
		//次のターンに進める
		if (_count >= _time) {
			SeqIndexAdd();
			_count = 0.0f;
		}

		break;
		//エネミー攻撃を受けるシーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE):
		if (cameras != Camera::enemycamera) {
			//敵カメラに切り替え
			playercontroller->SetViewTargetWithBlend(EnemyCamera, 1.0);
			cameras = Camera::enemycamera;
		}
		UKismetSystemLibrary::PrintString(this, "~BATTLE_ENEMYHIT~", true, true, FColor::Cyan, 2.f, TEXT("None"));

		//敵攻撃を受ける関数
		//enemy->TakeDamage(enemydamage);

		_count += DeltaTime;
		//次のターンに進める
		if (_count >= _time) {
			SeqIndexAdd();
			_count = 0.0f;
		}

		break;
		//バトルリザルト画面シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_RESULT):
		if (cameras != Camera::battlecamera) {
			//バトルシーンのカメラに切り替え
			playercontroller->SetViewTargetWithBlend(BattleSceneCamera, 1.0);
			cameras = Camera::battlecamera;
		}
		UKismetSystemLibrary::PrintString(this, "~BATTLE_RESULT~", true, true, FColor::Cyan, 2.f, TEXT("None"));
		
		_count += DeltaTime;
		//次のターンに進める
		if (_count >= _time) {
			SeqIndexAdd();
			_count = 0.0f;
		}
		
		break;
		//バトル終了シーケンス
	case std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_END):		
		UKismetSystemLibrary::PrintString(this, "~BATTLE_END~", true, true, FColor::Cyan, 2.f, TEXT("None"));

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

