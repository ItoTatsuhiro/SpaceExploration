// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleScene/BattleManager.h"
#include "Character/CharacterBase.h"
#include "Character/EnemyBase.h"
#include "Character/PlayerCharacter.h"
#include "MyGameInstance.h"
#include "BattleScene/E_BattleSEQ.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Manager/PlaySceneGameModeBase.h"
#include "BattleScene/BatllSceneWidget.h"
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

	if (nextlevel.IsNull()) {
		UE_LOG(LogClass, Warning, TEXT("NO nextlevel battlemanager beginplay\n"));
	}
	else {
		UE_LOG(LogClass, Log, TEXT("YES nextlevel battlemanager beginplay\n"));
	}

	//GameMode取得
	gamemode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(this));

	//カメラ切り替え用PlayerControllerを取得
	playercontroller = UGameplayStatics::GetPlayerController(this, 0);
	//ゲームインスタンス取得
	mygameinstance = Cast<UMyGameInstance>(GetGameInstance());

//キャラクター情報取得時の例外処理-----------------------------------------------------------------------

	//プレイヤー情報取得
	if (UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0)) {
		UE_LOG(LogClass, Log, TEXT("success playerstatus load\n"));
		//プレイヤーの情報取得
		player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
		//順番決めよう一時変数挿入
		playerstatus_ = player->GetCharacterStatus();
		provplayerstatus_.hp_ = playerstatus_.HP;
		//現在プレイヤーが武器を持っていないため属性が取得できない
		//provplayerstatus_.type_ = player->GetEquippedWeapon()->GetWeaponElement();
		provplayerstatus_.type_ = EElement::fire;
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("error : NO playerstatus\n"));
		//エラー落ちしないように仮のステータスを挿入
		playerstatus_.HP = 10.0f;
		playerstatus_.MaxHp = 30.0f;
		playerstatus_.AttackPower = 50.0f;
		playerstatus_.DefencePower = 20.0f;
		playerstatus_.Speed = 15.0f;
		provplayerstatus_.hp_ = playerstatus_.HP;
		provplayerstatus_.type_ = EElement::fire;
	}

	//エネミー情報取得時の例外処理
	if (mygameinstance->GetterBattleEnemyStatus().MaxHp <= 1.f) {
		UE_LOG(LogClass, Warning, TEXT("error : NO enemystatus\n"));
		//エラー落ちしないように仮のステータスを挿入
		enemystatus_.HP = 10.0f;
		enemystatus_.MaxHp = 30.0f;
		enemystatus_.AttackPower = 50.0f;
		enemystatus_.DefencePower = 20.0f;
		enemystatus_.Speed = 10.0f;
		provenemystatus_.hp_ = enemystatus_.HP;
	}
	else {
		UE_LOG(LogClass, Log, TEXT("success enemystatus load\n"));
		//エネミーのステータス、属性を取得
		enemystatus_ = mygameinstance->GetterBattleEnemyStatus();
		//順番決めようの一時変数挿入
		provenemystatus_.hp_ = enemystatus_.HP;
	}
	//エネミー属性取得時の例外処理
	if (mygameinstance->GetterBattleEnemyElement() == EElement::none) {
		UE_LOG(LogClass, Warning, TEXT("error : NO enemyelement\n"));
		provenemystatus_.type_ = EElement::wind;
	}
	else {
		UE_LOG(LogClass, Log, TEXT("success enemyelement load\n"));
		//エネミーのステータス、属性を取得
		provenemystatus_.type_ = mygameinstance->GetterBattleEnemyElement();
	}
	//敵生成
	FString bpenemypath = "/Game/Enemy/BP_EnemyType1.BP_EnemyType1_C";
	TSubclassOf<APawn> bpenemyclass = TSoftClassPtr<APawn>(FSoftObjectPath(*bpenemypath)).LoadSynchronous();
	if (bpenemyclass != nullptr) {
		//敵スポン
		enemy = Cast<AEnemyBase>(GetWorld()->SpawnActor<APawn>(bpenemyclass));
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("error enemyspawn\n"));
	}
	
//-----------------------------------------------------------------------------------------------------

	//カメラをバトルシーン全体を見る物に切り替え
	playercontroller->SetViewTargetWithBlend(BattleSceneCamera, 0.0);
	//プレイヤーのカメラと座標とサイズ設定
	if (player) {
		UE_LOG(LogClass, Log, TEXT("success playerCamera load\n"));
		//カメラ設定
		PlayerCamera = player->GetBattleCameraComponent()->GetChildActor();
		//座標設定
		player->SetCharacterLocation(playerpos_actor->GetActorLocation());
		UE_LOG(LogClass, Warning, TEXT("player pos : %f %f %f\n"),player->GetActorLocation().X, player->GetActorLocation().Y, player->GetActorLocation().Z);
		UE_LOG(LogClass, Warning, TEXT("playerpos_actor pos : %f %f %f\n"), playerpos_actor->GetActorLocation().X, playerpos_actor->GetActorLocation().Y, playerpos_actor->GetActorLocation().Z);
		//元の角度取得
		PlayerOrigineRotate = player->GetActorRotation();
		//プレイヤー回転
		player->SetActorRotation(PlayerBattleSceneRotate);

		UStaticMeshComponent* playermesh = player->FindComponentByClass<UStaticMeshComponent>();
		PlayerOriginSize = playermesh->Bounds.BoxExtent * 2.0f;
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("error : No player\n"));
	}

	//敵のカメラと座標設定
	if (enemy) {
		UE_LOG(LogClass, Log, TEXT("success enemy load\n"));
		//カメラ設定
		EnemyCamera = enemy->GetBattleCameraComponent()->GetChildActor();
		//座標設定
		enemy->SetCharacterLocation(enemypos_actor->GetActorLocation());
		UE_LOG(LogClass, Warning, TEXT("player pos : %f %f %f\n"), enemy->GetActorLocation().X, enemy->GetActorLocation().Y, enemy->GetActorLocation().Z);
		UE_LOG(LogClass, Warning, TEXT("playerpos_actor pos : %f %f %f\n"), enemypos_actor->GetActorLocation().X, enemypos_actor->GetActorLocation().Y, enemypos_actor->GetActorLocation().Z);
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("error : No enemyCamera\n"));
	}

//-----------------------------------------------------------------------------------------------------

	//Widget関係

	//widgetblueprintのclassを取得する
	FString BattleStartWidgetPath = TEXT("/Game/BattleScene/WBP_BattleEnd.WBP_BattleEnd_C");
	if (!BattleEndWidgetClass) {
		BattleEndWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*BattleStartWidgetPath)).LoadSynchronous();
	}
	
	if (BattleEndWidgetClass) {
		//バトル終了ボタン生成
		battleendwidget = UWidgetBlueprintLibrary::Create(GetWorld(), BattleEndWidgetClass, playercontroller);

		//バトル終了ボタンをAddViewportに追加
		battleendwidget->AddToViewport(0);
		
		//バトル終了ボタンを非表示
		battleendwidget->SetVisibility(ESlateVisibility::Hidden);

		//UBatllSceneWidgetにBattleManagerを持たせる
		UBatllSceneWidget* BattleWidgetRef = Cast<UBatllSceneWidget>(battleendwidget);
		if (BattleWidgetRef) {
			BattleWidgetRef->SetBattleManager(this);
		}
	}
	
//-----------------------------------------------------------------------------------------------------

	//バトル順など初期化
	ButtleInit();

	battlesequence.BindUObject(this, &ABattleManager::SEQ_BATTLE_STANDBY);
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
	UE_LOG(LogClass, Warning, TEXT("BattleTurn"));

	//ダメージの計算
	enemydamage = DamageMath(playerstatus_.AttackPower, static_cast<int>(provplayerstatus_.type_), enemystatus_.DefencePower, static_cast<int>(provenemystatus_.type_));
	playerdamage = DamageMath(enemystatus_.AttackPower, static_cast<int>(provenemystatus_.type_), playerstatus_.DefencePower, static_cast<int>(provplayerstatus_.type_));

	//プレイヤーか敵のどちらかの体力が0になるまで
	while (provenemystatus_.hp_ > 0.0f && provplayerstatus_.hp_ > 0.0f) {
		provplayerstatus_.attack_count_ += playerstatus_.Speed;
		provenemystatus_.attack_count_ += enemystatus_.Speed;

		//プレイヤーの攻撃
		if (provplayerstatus_.attack_count_ >= attack_timing_) {
			provplayerstatus_.attack_count_ = 0.0f;
			
			provenemystatus_.hp_ -= enemydamage;

			//順番を設定
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK));
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE));
		}

		//敵の攻撃
		if (provenemystatus_.attack_count_ >= attack_timing_) {
			provenemystatus_.attack_count_ = 0.0f;

			provplayerstatus_.hp_ -= playerdamage;

			//順番を設定
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACK));
			attack_order.emplace_back(std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACKRECEIVE));
		}
	}
	//勝者を決定
	if (provenemystatus_.hp_ <= 0.0f) {
		battlewinner = E_BatlleWinner::player;
	}
	else if (provplayerstatus_.hp_ <= 0.0f) {
		battlewinner = E_BatlleWinner::enemy;
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

bool ABattleManager::SEQ_BATTLE_STANDBY(const float deltatime)
{
	static bool once_seq_battle_standby = false;
	if (once_seq_battle_standby == false) {
		
		//バトルスタートボタン表示
		//battlestartwidget->AddToViewport(0);
		
		once_seq_battle_standby = true;
	}

	//UKismetSystemLibrary::PrintString(this, "~BATTLE_STANDBY~", true, true, FColor::Cyan, 2.f, TEXT("None"));

	_count += deltatime;
	//次のターンに進める
	if (_count >= _time) {
		once_seq_battle_standby = false;

		SEQChange_CameraChange();
		_count = 0.0f;
	}

	return true;
}

bool ABattleManager::SEQ_PLAYER_ATTACK(const float deltatime)
{
	static bool once_seq_player_attack = false;
	if (once_seq_player_attack == false) {

		once_seq_player_attack = true;
	}

	//UKismetSystemLibrary::PrintString(this, "~PLAYER_ATTACK~", true, true, FColor::Cyan, 2.f, TEXT("None"));

	//プレイヤー攻撃関数
	//player->StartAttackAction();

	playerpos_actor->AddActorLocalRotation(FQuat(1));

	_count += deltatime;
	//次のターンに進める
	if (_count >= _time) {
		once_seq_player_attack = false;

		SEQChange_CameraChange();
		_count = 0.0f;
	}

	return true;
}

bool ABattleManager::SEQ_PLAYER_ATTACKRECEIVE(const float deltatime)
{
	static bool once_seq_player_attackreceive = false;
	if (once_seq_player_attackreceive == false) {

		once_seq_player_attackreceive = true;
	}

	//UKismetSystemLibrary::PrintString(this, "~PLAYER_ATTACKRECEIVE~", true, true, FColor::Cyan, 2.f, TEXT("None"));

	//プレイヤー攻撃を受ける関数
	//player->TakeDamage(playerdamage);

	playerpos_actor->AddActorLocalRotation(FQuat(2));
		
	_count += deltatime;
	//次のターンに進める
	if (_count >= _time) {
		once_seq_player_attackreceive = false;

		SEQChange_CameraChange();
		_count = 0.0f;
	}

	return true;
}

bool ABattleManager::SEQ_ENEMY_ATTACK(const float deltatime)
{
	static bool once_seq_enemy_attack = false;
	if (once_seq_enemy_attack == false) {

		once_seq_enemy_attack = true;
	}

	//UKismetSystemLibrary::PrintString(this, "~ENEMY_ATTACK~", true, true, FColor::Cyan, 2.f, TEXT("None"));

	//エネミー攻撃
	//enemy->StartAttackAction();

	enemypos_actor->AddActorLocalRotation(FQuat(1));
	
	_count += deltatime;
	//次のターンに進める
	if (_count >= _time) {
		once_seq_enemy_attack = false;

		SEQChange_CameraChange();
		_count = 0.0f;
	}

	return true;
}

bool ABattleManager::SEQ_ENEMY_ATTACKRECEIVE(const float deltatime)
{
	static bool once_seq_enemy_attackreceive = false;
	if (once_seq_enemy_attackreceive == false) {

		once_seq_enemy_attackreceive = true;
	}

	//UKismetSystemLibrary::PrintString(this, "~ENEMY_ATTACKRECEIVE~", true, true, FColor::Cyan, 2.f, TEXT("None"));

	//エネミー攻撃を受ける
	//enemy->TakeDamage(enemydamage);

	enemypos_actor->AddActorLocalRotation(FQuat(2));

	_count += deltatime;
	//次のターンに進める
	if (_count >= _time) {
		once_seq_enemy_attackreceive = false;

		SEQChange_CameraChange();
		_count = 0.0f;
	}

	return true;
}

bool ABattleManager::SEQ_BATTLE_RESULT(const float deltatime)
{
	static bool once_seq_battle_result = false;
	if (once_seq_battle_result == false) {

		once_seq_battle_result = true;
	}

	//UKismetSystemLibrary::PrintString(this, "~BATTLE_RESULT~", true, true, FColor::Cyan, 2.f, TEXT("None"));

	if (battlewinner == E_BatlleWinner::player) {
		UKismetSystemLibrary::PrintString(this, "~Winner Player~", true, true, FColor::Cyan, 2.f, TEXT("None"));
		playerpos_actor->AddActorLocalRotation(FQuat(1));
	}
	else if (battlewinner == E_BatlleWinner::enemy) {
		UKismetSystemLibrary::PrintString(this, "~Winner Enemy~", true, true, FColor::Cyan, 2.f, TEXT("None"));
		enemypos_actor->AddActorLocalRotation(FQuat(1));
	}

	_count += deltatime;
	//次のターンに進める
	if (_count >= _time) {
		once_seq_battle_result = false;

		SEQChange_CameraChange();
		_count = 0.0f;
	}

	return true;
}

bool ABattleManager::SEQ_BATTLE_END(const float deltatime)
{
	static bool once_seq_battle_end = false;
	if (once_seq_battle_end == false) {
		//バトル終了ボタンを表示
		battleendwidget->SetVisibility(ESlateVisibility::Visible);

		once_seq_battle_end = true;
	}

	//UKismetSystemLibrary::PrintString(this, "~BATTLE_END~", true, true, FColor::Cyan, 2.f, TEXT("None"));

	once_seq_battle_end = false;

	player->SetActorRotation(PlayerOrigineRotate);

	////シーン移動
	//gamemode->ChangeLevel(nextlevel, this);

	return true;
}

void ABattleManager::SEQChange_CameraChange()
{
	//attack_orderのindexを次のターンへ移行
	SeqIndexAdd();
	//現在のシーケンスと次のシーケンスが別の物ならばカメラを切り替え
	if (NowBattleSeq != attack_order[seqindex]) {
		//プレイヤーのカメラに切り替え
		if (attack_order[seqindex] == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACK) ||
			attack_order[seqindex] == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::PLAYER_ATTACKRECEIVE)) {
			UE_LOG(LogClass, Warning, TEXT("SEQChange_CameraChange: camerachange player"));
			playercontroller->SetViewTargetWithBlend(PlayerCamera, camerachangetime);
		}
		//敵のカメラに切り替え
		else if (attack_order[seqindex] == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACK) ||
			attack_order[seqindex] == std::underlying_type<E_BattleSEQ>::type(E_BattleSEQ::ENEMY_ATTACKRECEIVE)) {
			UE_LOG(LogClass, Warning, TEXT("SEQChange_CameraChange: camerachange enemy"))
			playercontroller->SetViewTargetWithBlend(EnemyCamera, camerachangetime);
		}
		//バトルシーンのカメラに切り替え
		else {
			UE_LOG(LogClass, Warning, TEXT("SEQChange_CameraChange: camerachange battle"))
			playercontroller->SetViewTargetWithBlend(BattleSceneCamera, camerachangetime);
		}
	}
	//カメラ切り替え後、現在のシーケンスを設定
	NowBattleSeq = attack_order[seqindex];

	//camerachangetime経過後にシーケンス切り替え
	UWorld* world = GEngine->GameViewport->GetWorld();
	FTimerHandle timerhandle;
	world->GetTimerManager().SetTimer(timerhandle, this, &ABattleManager::SEQChange, camerachangetime, false);
}

void ABattleManager::SEQChange()
{
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

	//バトル終了ボタン消去
	battleendwidget->RemoveFromParent();
}
