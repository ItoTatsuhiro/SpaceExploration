// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Templates/Function.h"
#include "GameFramework/Actor.h"
#include "../Character/CharacterBase.h"
#include "../GameData/EnemyData.h"
#include "tsutsumi/Element.h"
#include "Library\GameLibrary.h"
#include "BattleManager.generated.h"

class APlayerCharacter;
class AEnemyBase;
struct FStatus;
class APlayerController;
class UNiagaraComponent;
class UNiagaraSystem;
class APlaySceneGameModeBase;
class UUserWidget;
class AEnemyManager;
class AStageDataManager;
class UTileData;
struct FStageMapData;

UCLASS()
class SPACEEXPLORATION_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleManager();
	~ABattleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
//---------------------------------------------------------------------------------------------
//バトルのターン決め関係

	//属性
	enum TYPE {
		fire,
		water,
		wind
	};
	//属性相性のダメージ補正値
	//good_ = 2.0
	//bad_ = 0.5
	//none_ = 1.0
	const float good_ = 2.0, bad_ = 0.5, none_ = 1.0;

	//戦闘するプレイヤーのステータス
	FStatus playerstatus_;
	//戦闘する敵のステータス
	FStatus enemystatus_;

	//攻撃順番判定用の一時変数
	struct character {
		float hp_ = 10;
	
		//属性　0＝火　1＝水　2＝風
		//属性相性　火＜水　水＜風　風＜火
		EElement type_;

		float attack_count_ = 0.0;
	};
	//攻撃順番判定用の一時変数（プレイヤー）
	character provplayerstatus_;
	//攻撃順番判定用の一時変数（エネミー）
	character provenemystatus_;
	
	//行動を決める値
	float attack_timing_ = 10;

//---------------------------------------------------------------------------------------------
 
	//バトルの流れ
private:
	//プレイヤーの情報
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerCharacter> player = nullptr;
	//敵の情報
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AEnemyBase> enemy = nullptr;
	//敵のBPのパス
	const FString BPPath_EnemyType1 = "/Game/Enemy/BP_EnemyType1.BP_EnemyType1_C";
	const FString BPPath_EnemyType2 = "/Game/Enemy/BP_EnemyType2.BP_EnemyType2_C";

	//勝敗用
	enum class E_BatlleWinner{
		none,
		player,
		enemy,
	};
	E_BatlleWinner battlewinner = E_BatlleWinner::none; 

	//受けるダメージ
	float playerdamage = 0.0, enemydamage = 0.0;

	//シーケンス時間
	const float _time = 2.0f;
	float _count = 0.0f;

	//カメラ切り替えに掛かる時間
	const float camerachangetime = 1.0f;
	//カメラ切り替えカウント
	float camerachangecount = 0.0f;

	//バトルで使用するプレイヤーのカメラ
	UPROPERTY(EditAnywhere, Category = "Camera")
	AActor* PlayerCamera = nullptr;
	//バトルで使用するエネミーのカメラ
	UPROPERTY(EditAnywhere, Category = "Camera")
	AActor* EnemyCamera = nullptr;
	//バトルシーンレベルのカメラ
	UPROPERTY(EditAnywhere, Category = "Camera")
	AActor* BattleSceneCamera = nullptr;
	enum class Camera {
		none,
		battlecamera,
		playercamera,
		enemycamera,
	};
	Camera cameras = Camera::none;

	//順番を入れる配列
	std::vector<uint8> attack_order;
	//現在のバトル順番のインデックス
	uint8 seqindex = 0;

	//各シーケンスで一度だけ処理する事の確認変数
	bool Seq_IsOnce_ = false;

	//止まっている戦闘惑星の情報
	UPROPERTY(VisibleAnywhere)
	FEnemyData battletiledata;

//---------------------------------------------------------------------------------------------
//Widget関係

	//バトル終了UI
	UPROPERTY()
	TSubclassOf<UUserWidget> BattleEndWidgetClass;
	UPROPERTY()
	UUserWidget* battleendwidget = nullptr;

	//ゲームオーバー終了UI
	UPROPERTY()
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY()
	UUserWidget* gameoberwidget = nullptr;

	//バトル開始UI
	UPROPERTY()
	TSubclassOf<UUserWidget> BattleStartWidgetClass;
	UPROPERTY()
	UUserWidget* battlestartwidget = nullptr;

	

public:
	//HPBar用HPの比率
	UPROPERTY(BlueprintReadOnly)
	float playerHP_ratio = 1.0f;
	UPROPERTY(BlueprintReadOnly)
	float enemyHP_ratio = 1.0f;

	//HPbarの上の属性
	UPROPERTY(BlueprintReadOnly)
	int playerelement = 1;
	UPROPERTY(BlueprintReadOnly)
	int enemyelement = 0;

	//バトル前のステータス
	UPROPERTY(BlueprintReadOnly)
	FStatus PostBattlePlayerStatus;
	//バトル後のステータス
	UPROPERTY(BlueprintReadOnly)
	FStatus PreBattlePlayerStatus;

	//Gameover時の表示物
	UPROPERTY(BlueprintReadOnly)
	int GameOverDisplayStatus_MapLevel = 1;
	UPROPERTY(BlueprintReadOnly)
	int GameOverDisplayStatus_PlayerLevel = 1;
	UPROPERTY(BlueprintReadOnly)
	int	GameOverDisplayStatus_WeaponElement = 0;
	UPROPERTY(BlueprintReadOnly)
	int GameOverDisplayStatus_WeaponLevel = 1;

//--------------------------------------------------------------------------------------------

public:

	//実行中のシーケンス
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BattleManager")
	uint8 NowBattleSeq = 0;

private:
	//プレイヤーのバトルシーン以外での角度
	FRotator PlayerOrigineRotate;
	//プレイヤーのバトルシーンでの角度
	FRotator PlayerBattleSceneRotate{0.0, -90.0, 0.0};
	//敵のバトルシーンでの角度
	FRotator EnemyBattleSceneRotate{ 0.0, 90.0, 0.0 };


	//ゲームモード
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlaySceneGameModeBase> gamemode;

	//プレイヤーコントローラー
	UPROPERTY(VisibleAnywhere)
	APlayerController* playercontroller = nullptr;

	//エネミーマネージャー
	TObjectPtr<AEnemyManager> enemymanager;

	//移動先のレベル
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> nextlevel = nullptr;

	//バトル時のプレイヤーと敵の座標を取得するためのActor
	UPROPERTY(EditAnywhere, Category = "CharactorPos")
	AActor* playerpos_actor;
	UPROPERTY(EditAnywhere, Category = "CharactorPos")
	AActor* enemypos_actor;

//関数
public:
	//バトルシーンに入った時の初期化関数
	UFUNCTION(BlueprintCallable, Category = "BattleManager")
	void ButtleInit();

	//攻撃の順番を決める
	void BattleTurn();

	//ダメージ計算
	//引数１：攻撃側の攻撃力
	//引数２：攻撃側の属性
	//引数３：防御側の防御力
	//引数４：防御側の属性
	float DamageMath(const float& A_atk, const EElement& A_type, const float& D_def, const EElement& D_type);

	Sequence battlesequence;
	//バトルシーケンス
	bool SEQ_BATTLE_STANDBY(const float deltatime);
	bool SEQ_PLAYER_ATTACK(const float deltatime);
	bool SEQ_PLAYER_ATTACKRECEIVE(const float deltatime);
	bool SEQ_ENEMY_ATTACK(const float deltatime);
	bool SEQ_ENEMY_ATTACKRECEIVE(const float deltatime);
	bool SEQ_BATTLE_RESULT(const float deltatime);
	bool SEQ_BATTLE_END(const float deltatime);
	bool SEQ_CAMERA_CHANGE(const float deltatime);

	void SEQChange();

	//バトルシーンを終了してマップシーンに戻る関数
	UFUNCTION()
	void BattleEnd();

	//現在のバトル順番を一つ進める(seqindexインクリメント)
	void SeqIndexAdd() { seqindex++; };


};
