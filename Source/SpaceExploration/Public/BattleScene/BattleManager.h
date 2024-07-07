// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"



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
	struct character {
		float hp_ = 0;
		float speed_ = 0.0;
		float defense_ = 0.0;
		float attack_ = 0.0;
		//属性　0＝火　1＝水　2＝風
		int _type_ = 0;

		float attack_count_ = 0.0;
	};

	character player_;
	character enemy_;

	//体力
	float player_hp_ = 30, enemy_hp_ = 20;
	//プレイヤーとエネミーの早さ
	float player_speed_ = 5.0f, enemy_speed_ = 3.0f;
	//

	//攻撃順番決めのカウント
	float player_attack_count_ = 0.0f, enemy_attack_count_ = 0.0f;
	//行動を決める値
	float attack_speed_ = 10;
	//順番を入れる配列
	std::vector<character> attack_order;

	//ダメージ計算
	void DamageMath();
	//攻撃の順番を決める
	void BattleTurn();
};
