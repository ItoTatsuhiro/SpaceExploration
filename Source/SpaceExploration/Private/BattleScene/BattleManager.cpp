// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleScene/BattleManager.h"
#include "Character/CharacterBase.h"
#include "Character/EnemyBase.h"
#include "Character/PlayerCharacter.h"
#include "BattleScene/E_BattleSEQ.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Manager/PlaySceneGameModeBase.h"
#include "Manager/EnemyManager.h"
#include "BattleScene/BatllSceneWidget.h"
#include "BattleScene/BattleStandoff_Widget.h"
#include "BattleScene/GameOverWidget.h"
#include "Character/E_CharacterActState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ABattleManager::ABattleManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ABattleManager::~ABattleManager()
{
	//生成した敵を消去
	if (enemy) {
		enemy->Destroy();
	}
}

// Called when the game starts or when spawned
void ABattleManager::BeginPlay()
{
	Super::BeginPlay();

	//各シーケンスで一度だけ処理する事の確認変数を初期化
	Seq_IsOnce_ = false;

	if (nextlevel.IsNull()) {
		UE_LOG(LogClass, Warning, TEXT("ABattleManager::BeginPlay : error : NO nextlevel\n"));
	}
	else {
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : success : YES nextlevel\n"));
	}

	//GameMode取得
	gamemode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!gamemode) {
		UE_LOG(LogClass, Warning, TEXT("ABattleManager::BeginPlay : error : NO gamemode\n"));
		UKismetSystemLibrary::QuitGame(GetWorld(), playercontroller, EQuitPreference::Quit, false);
	}
	else {
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : success : YES gamemode\n"));
	}

	//PlayerControllerを取得
	playercontroller = UGameplayStatics::GetPlayerController(this, 0);
	if (!playercontroller) {
		UKismetSystemLibrary::QuitGame(GetWorld(), playercontroller, EQuitPreference::Quit, false);
		UE_LOG(LogClass, Warning, TEXT("ABattleManager::BeginPlay : error : NO playercontroller\n"));
	}
	else {
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : success : YES playercontroller\n"));
	}

	if (gamemode) {
		//エネミーマネージャー取得
		enemymanager = gamemode->GetEnemyManager();
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : success : getEnemyManager\n"));
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("ABattleManager::BeginPlay : error : No getEnemyManager\n"));
		UKismetSystemLibrary::QuitGame(GetWorld(), playercontroller, EQuitPreference::Quit, false);
	}
	
//******************************************************************************
//敵のステータスはゲームインスタンスではなく、伊藤氏が作るStageManagerから受け取る
//敵をインスタンス化するのはEnemyManager（ParentLevelに設置）で行う
//******************************************************************************

//キャラクター情報取得時----------------------------------------------------------------------

//プレイヤー情報取得

	//プレイヤーの情報取得
	player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));

	if (player) {
		//順番決めよう一時変数挿入
		playerstatus_ = player->GetCharacterStatus();
		provplayerstatus_.hp_ = playerstatus_.HP;
		//プレイヤー属性取得
		provplayerstatus_.type_ = player->GetEquippedWeapon()->GetWeaponElement();
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : success : playerstatus load\n"));
	}
	else {
		//エラー落ちしないように仮のステータスを挿入
		playerstatus_.HP = 30.0f;
		playerstatus_.MaxHp = 30.0f;
		playerstatus_.AttackPower = 50.0f;
		playerstatus_.DefencePower = 20.0f;
		playerstatus_.Speed = 15.0f;
		provplayerstatus_.hp_ = playerstatus_.HP;
		provplayerstatus_.type_ = EElement::fire;
		UE_LOG(LogClass, Warning, TEXT("ABattleManager::BeginPlay : error : NO playerstatus\n"));
	}
	//現在のhpとmaxhpの比率
	playerHP_ratio = playerstatus_.HP / playerstatus_.MaxHp;
	//プレイヤーの属性をwidgetで取得できるように変数に保存
	playerelement = static_cast<int>(provplayerstatus_.type_);

//エネミー生成、情報取得

	//敵生成
	if (enemymanager) {
		//敵のステータス（CreateEnemyの引数）はStageManagerから取る
		enemy = enemymanager->CreateEnemy(0, 2, EElement::wind);
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : success : create enemy\n"));
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("ABattleManager::BeginPlay : error : no enemy create\n"));
	}
	//ステータス取得
	if (enemy) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : success : enemystatus load\n"));
		//エネミーのステータスを取得
		enemystatus_ = enemy->GetCharacterStatus();
		//順番決めようの一時変数挿入
		provenemystatus_.hp_ = enemystatus_.HP;
		//エネミーのステータス、属性を取得
		provenemystatus_.type_ = enemy->GetEquippedWeapon()->GetWeaponElement();
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("ABattleManager::BeginPlay : error : NO enemystatus\n"));
		//エラー落ちしないように仮のステータスを挿入
		enemystatus_.HP = 2.0f;
		enemystatus_.MaxHp = 2.0f;
		enemystatus_.AttackPower = 2.0f;
		enemystatus_.DefencePower = 1.0f;
		enemystatus_.Speed = 1.0f;
		provenemystatus_.hp_ = enemystatus_.HP;
		provenemystatus_.type_ = EElement::wind;
	}
	//現在のhpとmaxhpの比率
	enemyHP_ratio = enemystatus_.HP / enemystatus_.MaxHp;
	//エネミーの属性をwidgetで取得できるように変数に保存
	enemyelement = static_cast<int>(provenemystatus_.type_);
	
//-----------------------------------------------------------------------------------------------------
//カメラ設定、座標移動

	//カメラをバトルシーン全体を見る物に切り替え
	playercontroller->SetViewTargetWithBlend(BattleSceneCamera, 0.0);
	
	//プレイヤーのカメラと座標とサイズ設定
	if (player && playerpos_actor) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : success : playerCamera playerPos\n"));
		//カメラ設定
		PlayerCamera = player->GetBattleCameraComponent()->GetChildActor();
		//座標設定
		player->SetCharacterLocation(playerpos_actor->GetActorLocation());
		//元の角度取得
		PlayerOrigineRotate = player->GetActorRotation();
		//プレイヤー回転
		player->SetActorRelativeRotation(PlayerBattleSceneRotate);
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("ABattleManager::BeginPlay : error : playerCamera playerPos\n"));
	}

	//敵のカメラと座標設定
	if (enemy && enemypos_actor) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : success : enemyCamera enemyPos\n"));
		//カメラ設定
		EnemyCamera = enemy->GetBattleCameraComponent()->GetChildActor();
		//座標設定
		enemy->SetCharacterLocation(enemypos_actor->GetActorLocation());
		//向き変更
		enemy->SetActorRelativeRotation(EnemyBattleSceneRotate);
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("ABattleManager::BeginPlay : error : enemyCamera enemyPos\n"));
	}

//-----------------------------------------------------------------------------------------------------
//Widget関係

	//バトル終了(勝利)のwidgetblueprintのclassを取得する
	FString BattleEndWidgetPath = TEXT("/Game/BattleScene/WBP_BattleEnd.WBP_BattleEnd_C");
	if (!BattleEndWidgetClass) {
		BattleEndWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*BattleEndWidgetPath)).LoadSynchronous();
	}
	if (BattleEndWidgetClass) {
		//バトル終了生成
		battleendwidget = UWidgetBlueprintLibrary::Create(GetWorld(), BattleEndWidgetClass, playercontroller);

		//バトル終了をAddViewportに追加
		battleendwidget->AddToViewport(0);
		
		//バトル終了を非表示
		battleendwidget->SetVisibility(ESlateVisibility::Hidden);

		//UBatllSceneWidgetにBattleManagerを持たせる
		UBatllSceneWidget* BattleWidgetRef = Cast<UBatllSceneWidget>(battleendwidget);
		if (BattleWidgetRef) {
			BattleWidgetRef->SetBattleManager(this);
		}

		//UIに表示するバトル前のプレイヤーのステータス
		PreBattlePlayerStatus = player->GetCharacterStatus();
	}

	//ゲームオーバーのwidgetblueprintのclassを取得する
	FString GameOverWidgetPath = TEXT("/Game/BattleScene/WBP_GameOver.WBP_GameOver_C");
	if (!GameOverWidgetClass) {
		GameOverWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*GameOverWidgetPath)).LoadSynchronous();
	}
	if (GameOverWidgetClass) {
		//ゲームオーバー生成
		gameoberwidget = UWidgetBlueprintLibrary::Create(GetWorld(), GameOverWidgetClass, playercontroller);

		//ゲームオーバーをAddViewportに追加
		gameoberwidget->AddToViewport(0);

		//ゲームオーバーを非表示
		gameoberwidget->SetVisibility(ESlateVisibility::Hidden);

		//UGameOverWidgetにBattleManagerを持たせる
		UGameOverWidget* gameoverref = Cast<UGameOverWidget>(gameoberwidget);
		if (gameoverref) {
			gameoverref->SetBattleManager(this);
		}

		//widgetに表示するステータスを設定
		GameOverDisplayStatus_PlayerLevel = player->GetCharacterStatus().PlayerLevel;
		GameOverDisplayStatus_WeaponLevel = player->GetEquippedWeapon()->GetWeaponStatus().PlayerLevel;
		GameOverDisplayStatus_MapLevel = 1;
		GameOverDisplayStatus_WeaponElement = static_cast<int>(player->GetEquippedWeapon()->GetWeaponElement());
	}

	//バトルスタートのwidgetbluprintのclassを取得する
	FString BattleStartWidgetPath = TEXT("/Game/BattleScene/WBP_BattleStandoff.WBP_BattleStandoff_C");
	if (!BattleStartWidgetClass) {
		BattleStartWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*BattleStartWidgetPath)).LoadSynchronous();
	}
	if (BattleStartWidgetClass) {
		//バトルスタートwidget生成
		battlestartwidget = UWidgetBlueprintLibrary::Create(GetWorld(), BattleStartWidgetClass, playercontroller);

		//バトルスタートwidgetをAddviewportに追加
		battlestartwidget->AddToViewport(0);

		//UBattleStandoff_WidgetにBattleManagerを持たせる
		UBattleStandoff_Widget* BattleStandoff = Cast<UBattleStandoff_Widget>(battlestartwidget);
		if (BattleStandoff) {
			BattleStandoff->SetBattleManager(this);
		}
	}
	
//-----------------------------------------------------------------------------------------------------

	//バトル順など初期化
	ButtleInit();

	for (int i = 0; i < attack_order.size(); i++) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager::BeginPlay : attack_order : %d : %d\n"),i,attack_order[i]);

	}
}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	battlesequence.Execute(DeltaTime);
}

void ABattleManager::ButtleInit()
{
	UE_LOG(LogClass, Warning, TEXT("ButtleInit"));

	//攻撃順初期化
	attack_order.clear();
	//attack_orderの始めにバトル前の準備シーンを設定
	attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_STANDBY));
	battlesequence.BindUObject(this, &ABattleManager::SEQ_BATTLE_STANDBY);

	//現在のバトル順初期化
	seqindex = 0;
	NowBattleSeq = attack_order[seqindex];

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

	UE_LOG(LogClass, Log, TEXT("ABattleManager : BattleTurn : PlayerAttackPawer%f PlayerDefencePower%f"), playerstatus_.AttackPower, playerstatus_.DefencePower);
	UE_LOG(LogClass, Log, TEXT("ABattleManager : BattleTurn : EnemyAttackPawer%f EnemyDefencePower%f"), enemystatus_.AttackPower, enemystatus_.DefencePower);
	UE_LOG(LogClass, Log, TEXT("ABattleManager : BattleTurn : PlayerTakeDamage%f EnemyTakeDamage%f"),playerdamage, enemydamage);

	//プレイヤーか敵のどちらかの体力が0になるまで
	while (provenemystatus_.hp_ > 0.0f && provplayerstatus_.hp_ > 0.0f) {
		provplayerstatus_.attack_count_ += playerstatus_.Speed;
		provenemystatus_.attack_count_ += enemystatus_.Speed;

		//プレイヤーの攻撃
		if (provplayerstatus_.attack_count_ >= attack_timing_ && provenemystatus_.hp_ > 0.0f) {
			provplayerstatus_.attack_count_ = 0.0f;
			
			provenemystatus_.hp_ -= enemydamage;

			//順番を設定
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK));
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE));
		}

		//敵の攻撃
		if (provenemystatus_.attack_count_ >= attack_timing_ && provenemystatus_.hp_ > 0.0f) {
			provenemystatus_.attack_count_ = 0.0f;

			provplayerstatus_.hp_ -= playerdamage;

			//順番を設定
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACK));
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACKRECEIVE));
		}
	}

	//勝者を決定

	if (provplayerstatus_.hp_ <= 0.0f) {
		battlewinner = E_BatlleWinner::enemy;
	}
	else if (provenemystatus_.hp_ <= 0.0f) {
		battlewinner = E_BatlleWinner::player;
	}
	
	//バトル終了シーケンス設定
	attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_RESULT));
	attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_END));
}

float ABattleManager::DamageMath(const float& A_atk, const EElement& A_type, const float& D_def, const EElement& D_type){
	//ダメージ
	int damage = 0;

	//相性判定
	if (A_type == EElement::fire) {
		switch (D_type)
		{
		case EElement::fire:
			damage = (A_atk / D_def) * none_;
			break;
		case EElement::water:
			damage = (A_atk / D_def) * bad_;
			break;
		case EElement::wind:
			damage = (A_atk / D_def) * good_;
			break;
		default:
			damage = (A_atk / D_def) * none_;
			break;
		}
	}
	else if (A_type == EElement::water) {
		switch (D_type)
		{
		case EElement::fire:
			damage = (A_atk / D_def) * good_;
			break;
		case EElement::water:
			damage = (A_atk / D_def) * none_;
			break;
		case EElement::wind:
			damage = (A_atk / D_def) * bad_;
			break;
		default:
			damage = (A_atk / D_def) * none_;
			break;
		}
	}
	else if (A_type == EElement::wind) {
		switch (D_type)
		{
		case EElement::fire:
			damage = (A_atk / D_def) * bad_;
			break;
		case EElement::water:
			damage = (A_atk / D_def) * good_;
			break;
		case EElement::wind:
			damage = (A_atk / D_def) * none_;
			break;
		default:
			damage = (A_atk / D_def) * none_;
			break;
		}
	}

	return damage;
}

bool ABattleManager::SEQ_BATTLE_STANDBY(const float deltatime)
{
	if (Seq_IsOnce_ == false) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_BATTLE_STANDBY start"))
	
		Seq_IsOnce_ = true;
	}

	_count += deltatime;
	//次のターンに進める
	if (_count >= _time) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_BATTLE_STANDBY end"))
		Seq_IsOnce_ = false;

		battlesequence.BindUObject(this, &ABattleManager::SEQ_CAMERA_CHANGE);
	}

	return true;
}

bool ABattleManager::SEQ_PLAYER_ATTACK(const float deltatime)
{
	if (Seq_IsOnce_ == false) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_PLAYER_ATTACK start"))

		//プレイヤー攻撃関数
		player->Attack();

		Seq_IsOnce_ = true;
	}

	//次のターンに進める
	if (player->GetCharaterActState() == ECharacterActState::Idle) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_PLAYER_ATTACK end"))
		Seq_IsOnce_ = false;

		battlesequence.BindUObject(this, &ABattleManager::SEQ_CAMERA_CHANGE);
	}

	return true;
}

bool ABattleManager::SEQ_PLAYER_ATTACKRECEIVE(const float deltatime)
{
	if (Seq_IsOnce_ == false) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_PLAYER_ATTACKRECEIVE start"))

		//プレイヤー攻撃を受ける関数
		player->TakeDamage(playerdamage);

		playerstatus_.HP -= playerdamage;
		playerHP_ratio = playerstatus_.HP / playerstatus_.MaxHp;

		Seq_IsOnce_ = true;
	}

	//次のターンに進める
	if (player->GetCharaterActState() == ECharacterActState::Idle) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_PLAYER_ATTACKRECEIVE end"))
		Seq_IsOnce_ = false;
		battlesequence.BindUObject(this, &ABattleManager::SEQ_CAMERA_CHANGE);
	}

	return true;
}

bool ABattleManager::SEQ_ENEMY_ATTACK(const float deltatime)
{
	if (Seq_IsOnce_ == false) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_ENEMY_ATTACK start"))

		//エネミー攻撃
		enemy->Attack();

		Seq_IsOnce_ = true;
	}

	//次のターンに進める
	if (enemy->GetCharaterActState() == ECharacterActState::Idle) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_ENEMY_ATTACK end"))
		Seq_IsOnce_ = false;
		battlesequence.BindUObject(this, &ABattleManager::SEQ_CAMERA_CHANGE);
	}

	return true;
}

bool ABattleManager::SEQ_ENEMY_ATTACKRECEIVE(const float deltatime)
{
	if (Seq_IsOnce_ == false) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_ENEMY_ATTACKRECEIVE start"))

		//エネミー攻撃を受ける
		enemy->TakeDamage(enemydamage);

		enemystatus_.HP -= enemydamage;
		enemyHP_ratio = enemystatus_.HP / enemystatus_.MaxHp;

		Seq_IsOnce_ = true;
	}

	//次のターンに進める
	if (enemy->GetCharaterActState() == ECharacterActState::Idle) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_ENEMY_ATTACKRECEIVE end"))
		Seq_IsOnce_ = false;
		battlesequence.BindUObject(this, &ABattleManager::SEQ_CAMERA_CHANGE);
	}

	return true;
}

bool ABattleManager::SEQ_BATTLE_RESULT(const float deltatime)
{
	if (Seq_IsOnce_ == false) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_BATTLE_RESULT start"))

		if (battlewinner == E_BatlleWinner::player) {
			player->AddExp(enemy->GetCharacterStatus().Exp);
			if (player->CanLevelUp()) {
				player->ExecuteLevelUp();
			}	
		}
		else{
		}

		//バトル後のプレイヤーステータス
		PostBattlePlayerStatus = player->GetCharacterStatus();

		battlestartwidget->SetVisibility(ESlateVisibility::Hidden);
		Seq_IsOnce_ = true;
	}

	_count += deltatime;
	//次のターンに進める
	if (_count >= _time) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_BATTLE_RESULT end"))
		Seq_IsOnce_ = false;
		_count = 0.0f;
		battlesequence.BindUObject(this, &ABattleManager::SEQ_CAMERA_CHANGE);
		
	}

	return true;
}

bool ABattleManager::SEQ_BATTLE_END(const float deltatime)
{
	if (Seq_IsOnce_ == false) {
		UE_LOG(LogClass, Log, TEXT("ABattleManager : SEQ_BATTLE_END start"))

		if (battlewinner == E_BatlleWinner::player) {
			UKismetSystemLibrary::PrintString(this, "~Winner Player~", true, true, FColor::Cyan, 2.f, TEXT("None"));

			//バトル終了（勝利）を表示
			battleendwidget->SetVisibility(ESlateVisibility::Visible);
		}
		else if (battlewinner == E_BatlleWinner::enemy) {
			UKismetSystemLibrary::PrintString(this, "~Winner Enemy~", true, true, FColor::Cyan, 2.f, TEXT("None"));

			//ゲームオーバーを表示
			gameoberwidget->SetVisibility(ESlateVisibility::Visible);
		}

		Seq_IsOnce_ = true;
	}

	return true;
}

bool ABattleManager::SEQ_CAMERA_CHANGE(const float deltatime)
{
	if (Seq_IsOnce_ == false) {
		//attack_orderのindexを次のターンへ移行
		SeqIndexAdd();
		//プレイヤーのカメラに切り替え
		if (attack_order[seqindex] == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK) ||
			attack_order[seqindex] == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACKRECEIVE)) {
			UE_LOG(LogClass, Warning, TEXT("SEQ_CAMERA_CHANGE: camerachange player"));
			playercontroller->SetViewTargetWithBlend(PlayerCamera, camerachangetime);
		}
		//敵のカメラに切り替え
		else if (attack_order[seqindex] == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACK) ||
			attack_order[seqindex] == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE)) {
			UE_LOG(LogClass, Warning, TEXT("SEQ_CAMERA_CHANGE: camerachange enemy"))
			playercontroller->SetViewTargetWithBlend(EnemyCamera, camerachangetime);
		}
		//バトルシーンのカメラに切り替え
		else {
			UE_LOG(LogClass, Warning, TEXT("SEQ_CAMERA_CHANGE: camerachange battle"))
			playercontroller->SetViewTargetWithBlend(BattleSceneCamera, camerachangetime);
		}

		Seq_IsOnce_ = true;
	}

	camerachangecount += deltatime;
	if (camerachangecount >= camerachangetime) {
		//カメラ切り替え後、現在のシーケンスを設定
		NowBattleSeq = attack_order[seqindex];
		camerachangecount = 0;
		Seq_IsOnce_ = false;

		//シーケンス切り替え
		SEQChange();
	}

	return false;
}

void ABattleManager::SEQChange()
{
	UE_LOG(LogClass, Log, TEXT("ABattleManager::SEQChange : success : SEQchange"));

	//次のシーケンスに切り替え
	if (NowBattleSeq == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK)) {
		battlesequence.BindUObject(this, &ABattleManager::SEQ_PLAYER_ATTACK);
	}
	else if (NowBattleSeq == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACKRECEIVE)) {
		battlesequence.BindUObject(this, &ABattleManager::SEQ_PLAYER_ATTACKRECEIVE);
	}
	else if (NowBattleSeq == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACK)) {
		battlesequence.BindUObject(this, &ABattleManager::SEQ_ENEMY_ATTACK);
	}
	else if (NowBattleSeq == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE)) {
		battlesequence.BindUObject(this, &ABattleManager::SEQ_ENEMY_ATTACKRECEIVE);
	}
	else if (NowBattleSeq == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_RESULT)) {
		battlesequence.BindUObject(this, &ABattleManager::SEQ_BATTLE_RESULT);
	}
	else if (NowBattleSeq == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::BATTLE_END)) {
		battlesequence.BindUObject(this, &ABattleManager::SEQ_BATTLE_END);
	}
}

void ABattleManager::BattleEnd()
{
	//シーン移動
	gamemode->ChangeLevel(nextlevel, this);

	//プレイヤーの向きを元に戻す
	player->SetActorRelativeRotation(PlayerOrigineRotate);

	//バトル終了UI消去
	battleendwidget->RemoveFromParent();
	//バトルスタートUI消去
	battlestartwidget->RemoveFromParent();
	//ゲームオーバーUI消去
	gameoberwidget->RemoveFromParent();
}
