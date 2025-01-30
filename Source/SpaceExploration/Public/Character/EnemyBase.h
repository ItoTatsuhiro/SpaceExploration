// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "../tsutsumi/Element.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPACEEXPLORATION_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------------------------------------------
	// 攻撃処理を行う
	// --------------------------------------------------------------------------
	void Attack() override;

	// --------------------------------------------------------------------------
	// ダメージを受ける処理を行う
	// 
	// ・引数
	// Damage：ダメージ量
	// --------------------------------------------------------------------------
	void TakeDamage(int32 Damage) override;

	// --------------------------------------------------------------------------
	// 死亡時の処理を行う
	// --------------------------------------------------------------------------
	void Death() override;

protected:
	// 敵の各行動シーケンスの処理を行うデリゲート
	Sequence EnemyActionSequence;

	// スケルタルメッシュコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> CharacterMeshComp;

	// 敵の属性
	UPROPERTY(EditAnywhere)
	EElement EnemyElement;

private:
	/* 
	* ======================================================================================
	* 以下は、敵の各行動シーケンスの処理を行う関数
	* 
	* delta_time...
	* 
	* 戻り値...true：成功、false：失敗
	* ======================================================================================
	*/

	// --------------------------------------------------------------------------
	// 待機行動シーケンス
	// --------------------------------------------------------------------------
	bool SeqStandbyAction(const float delta_time);

	// --------------------------------------------------------------------------
	// 攻撃行動シーケンス
	// --------------------------------------------------------------------------
	bool SeqAttackAction(const float delta_time);

	// --------------------------------------------------------------------------
	// ダメージを受けるシーケンス
	// --------------------------------------------------------------------------
	bool SeqTakeDamage(const float DeltaTime);

	// --------------------------------------------------------------------------
	// 死亡の演出を行うシーケンス
	// --------------------------------------------------------------------------
	bool SeqDeath(const float DeltaTime);

};
