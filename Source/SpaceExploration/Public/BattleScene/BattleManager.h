// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Character/CharacterBase.h"
#include "Library\GameLibrary.h"
#include "BattleManager.generated.h"

class APlayerCharacter;
class AEnemyBase;
class ALevelInterface;
struct FStatus;
class APlayerController;
class UNiagaraComponent;
class UNiagaraSystem;
class UMyGameInstance;
class APlaySceneGameModeBase;

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
	//属性相性判定用
	const int type_corr_[5] = { wind, fire, water, wind, fire };

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
		uint8 type_ = 0;

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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerCharacter> player = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AEnemyBase> enemy = nullptr;
	UPROPERTY(VisibleAnywhere)
	ALevelInterface* levelinterface = nullptr;
	//UPROPERTY(VisibleAnywhere)

	//勝敗用
	enum class E_BatlleWinner{
		none,
		player,
		enemy,
	};
	E_BatlleWinner battlewinner = E_BatlleWinner::none;

	//カメラ切り替え用
	APlayerController* playercontroller = nullptr;

	//受けるダメージ
	float playerdamage = 0.0, enemydamage = 0.0;

	//シーケンス時間
	float _time = 2.0f, _count = 0.0f;

	//バトルで使用するプレイヤーのカメラ
	UPROPERTY(EditAnywhere, Category = "Camera")
	AActor* PlayerCamera = nullptr;
	//バトルで使用するエネミーのカメラ
	UPROPERTY(EditAnywhere, Category = "Camera")
	AActor* EnemyCamera = nullptr;

	//バトルシーンのカメラ
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

	//バトルシーンレベル
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> MyLevel;
	//バトルシーンレベルのカメラ
	UPROPERTY(EditAnywhere, Category = "Camera")
	AActor* BattleSceneCamera = nullptr;

public:
	//実行中のシーケンス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleManager")
	uint8 NowBattleSeq = 0;

private:
	//ゲームインスタンス
	UPROPERTY(VisibleAnywhere)
	UMyGameInstance* mygameinstance = nullptr;

	//ゲームモード
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlaySceneGameModeBase> gamemode;

	//移動先のレベル
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> nextlevel = nullptr;

	//バトル時のプレイヤーと敵の座標を取得するためのActor
	UPROPERTY(EditAnywhere, Category = "CharactorPos")
	AActor* playerpos_actor;
	UPROPERTY(EditAnywhere, Category = "CharactorPos")
	AActor* enemypos_actor;

//エフェクト（Niagara）関係
	//敵のエフェクト
	//攻撃　炎
	UPROPERTY(EditAnywhere, Category = "particl|enemy")
	UNiagaraComponent* niagara_enemy_attack_fier;
	//攻撃　水
	UPROPERTY(EditAnywhere, Category = "particl|enemy")
	UNiagaraComponent* niagara_enemy_attack_water;
	//攻撃　風
	UPROPERTY(EditAnywhere, Category = "particl|enemy")
	UNiagaraComponent* niagara_enemy_attack_wind;
	//攻撃
	UPROPERTY(EditAnywhere, Category = "particl|enemy")
	UNiagaraComponent* niagara_enemy_attack;
	//攻撃ヒット
	UPROPERTY(EditAnywhere, Category = "particl|enemy")
	UNiagaraComponent* niagara_enemy_hitreceive;
	//死亡
	UPROPERTY(EditAnywhere, Category = "particl|enemy")
	UNiagaraComponent* niagara_enemy_death;

	//プレイヤーのエフェクト
	//攻撃　炎
	UPROPERTY(EditAnywhere, Category = "particl|player")
	UNiagaraComponent* niagara_player_attack_fier;
	//攻撃　水
	UPROPERTY(EditAnywhere, Category = "particl|player")
	UNiagaraComponent* niagara_player_attack_water;
	//攻撃　風
	UPROPERTY(EditAnywhere, Category = "particl|player")
	UNiagaraComponent* niagara_player_attack_wind;
	//攻撃
	UPROPERTY(EditAnywhere, Category = "particl|player")
	UNiagaraComponent* niagara_player_attack;
	//攻撃ヒット
	UPROPERTY(EditAnywhere, Category = "particl|player")
	UNiagaraComponent* niagara_player_hitreceive;
	//死亡
	UPROPERTY(EditAnywhere, Category = "particl|player")
	UNiagaraComponent* niagara_player_death;

//関数

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
	float DamageMath(const float& A_atk, const int& A_type, const float& D_def, const int& D_type);

	Sequence battlesequence;
	//バトルシーケンス
	bool CAMERACHANGE(const float deltatime);
	bool BATTLE_STANDBY(const float deltatime);
	bool PLAYER_ATTACK(const float deltatime);
	bool PLAYER_ATTACKRECEIVE(const float deltatime);
	bool ENEMY_ATTACK(const float deltatime);
	bool ENEMY_ATTACKRECEIVE(const float deltatime);
	bool BATTLE_RESULT(const float deltatime);
	bool BATTLE_END(const float deltatime);

	//現在のバトル順番を一つ進める
	void SeqIndexAdd() { seqindex++; };
	
};
