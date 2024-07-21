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
	//属性
	enum TYPE {
		fire,
		water,
		wind
	};
	
	//属性相性判定用
	int type_corr_[5] = { wind, fire, water, wind, fire };

	//属性相性のダメージ補正値
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

	enum class CHARACTER {
		player,
		enemy,
	};

	//行動を決める値
	float attack_speed_ = 10;
	//順番を入れる配列
	std::vector<CHARACTER> attack_order;

public:
	//攻撃の順番を決める
	void BattleTurn();

	//ダメージ計算
	//引数１・攻撃側の攻撃力
	//引数２・攻撃側の属性
	//引数３・防御側の防御力
	//引数４・防御側の属性
	float DamageMath(float A_atk, int A_type, float D_def, int D_type);
};
