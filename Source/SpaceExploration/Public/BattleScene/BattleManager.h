// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BattleManager.generated.h"

class APlayerCharacter;
class AEnemyBase;
class ALevelInterface;
struct FStatus;

UCLASS()
class SPACEEXPLORATION_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleManager();

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
	int type_corr_[5] = { wind, fire, water, wind, fire };
	//属性相性のダメージ補正値
	//good_ = 2.0
	//bad_ = 0.5
	//none_ = 1.0
	const float good_ = 2.0, bad_ = 0.5, none_ = 1.0;
	struct character {
		float hp_ = 0;
		float speed_ = 0.0;
		float defense_ = 0.0;
		float attack_ = 0.0;
		//属性　0＝火　1＝水　2＝風
		//属性相性　火＜水　水＜風　風＜火
		int type_ = 0;

		float attack_count_ = 0.0;
	};
	character player_;
	character enemy_;
	
	//行動を決める値
	float attack_timing_ = 10;

	//受けるダメージ
	float playerdamage = 0.0, enemydamage = 0.0;
//---------------------------------------------------------------------------------------------
 
	//バトルの流れ
private:
	APlayerCharacter* player;
	AEnemyBase* enemy;
	ALevelInterface* levelinterface;

	//順番を入れる配列
	std::vector<uint8> attack_order;
	//現在の順番のインデックス
	uint8 seqindex = 0;

	//バトル終了後のレベル
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> NextLevel;
public:
	//実行中のシーケンス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleManager")
	uint8 NowBattleSeq = 0;

public:
	//BPで使用するための配列
	UPROPERTY(BlueprintReadOnly, Category = "BattleManager")
	TArray<uint8> ConvertArray;

//関数
	//バトルシーンに入った時の初期化関数
	//引数１：プレイヤーのステータスデータ
	//引数２：プレイヤーの属性
	//引数３：敵のステータスデータ
	//引数４：敵の属性
	UFUNCTION(BlueprintCallable, Category = "BattleManager")
	void ButtleInit(const  FStatus& player, const int& playerelement, const FStatus& enemy, const int& enemyelement);

	//攻撃の順番を決める
	void BattleTurn();

	//ダメージ計算
	//引数１：プレイヤーの攻撃力
	//引数２：プレイヤーの属性
	//引数３：敵の防御力
	//引数４：敵の属性
	float DamageMath(const float& A_atk, const int& A_type, const float& D_def, const int& D_type);
	
//ゲッター
	//ターン取得
	std::vector<uint8> GetButtleTurn() { return attack_order; };

	//vectorを変換する関数
	UFUNCTION(BlueprintCallable, Category = "BattleManager")
	void ConvertVectorToActor();
};
