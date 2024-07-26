// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../tsutsumi/Status.h"
#include "../Weapon/WeaponBase.h"
#include "CharacterBase.generated.h"

DECLARE_DELEGATE_RetVal_OneParam(bool, Sequence, float);

UCLASS(BlueprintType, Blueprintable, Abstract)
class SPACEEXPLORATION_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 攻撃関数
	virtual void Attack() PURE_VIRTUAL(ACharacterBase::Attack, );

	// ----------------------------------------------------------------
	// ゲッター
	// ----------------------------------------------------------------

	// 生存しているか判定を返す
	UFUNCTION(BlueprintCallable)
	inline bool IsAlive() const { return CharacterStatus.HP > 0; }
	// キャラクターステータスを取得する
	UFUNCTION(BlueprintCallable)
	inline FStatus& GetCharacterStatus() { return CharacterStatus; }
	// キャラクターの位置を取得
	// 装備中の武器を取得
	UFUNCTION(BlueprintCallable)
	AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; };

	// ----------------------------------------------------------------
	// セッター
	// ----------------------------------------------------------------

	// キャラクターの位置を設定する。
	UFUNCTION(BlueprintCallable)
	inline void SetCharacterLocation(const FVector& Location) 
	{
		GetOwner()->SetActorLocation(Location);
	}

	// ステータスをセットする。
	UFUNCTION(BlueprintCallable)
	inline void SetCharacterStatus(FStatus Status) 
	{ 
		CharacterStatus = Status; 
	}
	
	// 装備する武器を設定する。
	UFUNCTION(BlueprintCallable)
	void SetEquippedWeapon(AWeaponBase* Weapon);

	// ----------------------------------------------------------------
	// その他
	// ----------------------------------------------------------------

	// キャラクターのHPを回復させて、回復量を返す関数
	// 引数：回復する値（デフォルト値の場合、全回復する）
	// 戻り値：回復した値
	UFUNCTION(BlueprintCallable)
	int32 RecoverHP(int32 RecoveryAmount = -1);

	// ダメージを受ける
	// 引数：ダメージ量
	UFUNCTION(BlueprintCallable)
	void TakeDamage(int32 Damage);

protected:
	// キャラクターのルートコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// ステータス
	UPROPERTY(EditAnywhere)
	FStatus CharacterStatus;

	// 装備中のウェポン
	UPROPERTY(EditAnywhere)
	TObjectPtr<AWeaponBase> EquippedWeapon;

};