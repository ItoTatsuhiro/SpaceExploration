// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../tsutsumi/Status.h"
#include "../Weapon/WeaponBase.h"
#include "../tsutsumi/Element.h"
#include "E_CharacterActState.h"
#include "../Library/GameLibrary.h"
#include "CharacterBase.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class SPACEEXPLORATION_API ACharacterBase : public APawn
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

	// ----------------------------------------------------------------
	// 攻撃関数
	// ----------------------------------------------------------------
	virtual void Attack() PURE_VIRTUAL(ACharacterBase::Attack, );

	// =========================================================================
	// ゲッター
	// =========================================================================

	// ----------------------------------------------------------------
	// 生存しているか判定を返す
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	inline bool IsAlive() const { return CharacterStatus.HP > 0; }

	// ----------------------------------------------------------------
	// キャラクターステータスを取得する
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	inline FStatus& GetCharacterStatus() { return CharacterStatus; }
	
	// ----------------------------------------------------------------
	// 装備中の武器を取得
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	UChildActorComponent* GetEquippedWeaponComp() { return EquippedWeaponComp; }

	// ----------------------------------------------------------------
	// 装備中の武器を取得
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	AWeaponBase* GetEquippedWeapon() const { return nullptr; };

	// ----------------------------------------------------------------
	// 装備中の武器を取得
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	EElement GetAttackElement() const;

	// ----------------------------------------------------------------
	// バトルシーンのカメラアクターコンポーネント
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	UChildActorComponent* GetBattleCameraComponent() const { return BattleCameraComp; }

	// ----------------------------------------------------------------
	// キャラクターの行動状態を取得する
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	ECharacterActState GetCharaterActState() const { return E_CharacterActState; }

	// =========================================================================
	// セッター
	// =========================================================================

	// ----------------------------------------------------------------
	// キャラクターの位置を設定する。
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	inline void SetCharacterLocation(const FVector& Location) 
	{
		SetActorLocation(Location);
	}

	// ----------------------------------------------------------------
	// ステータスをセットする。
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	inline void SetCharacterStatus(const FStatus& Status) 
	{ 
		CharacterStatus = Status; 
	}
	
	// ----------------------------------------------------------------
	// 装備する武器を設定する。
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void SetEquippedWeapon(AWeaponBase* Weapon);

	// =========================================================================
	// その他
	// =========================================================================

	// ----------------------------------------------------------------
	// キャラクターのHPを回復させて、回復量を返す関数
	// 
	// ・引数
	// RecoveryAmount：回復する値（デフォルト値の場合、全回復する）
	// 
	// 戻り値：回復した値
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	int32 RecoverHP(int32 RecoveryAmount = -1);

	// ----------------------------------------------------------------
	// 攻撃行動を開始
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void StartAttackAction();

	// ----------------------------------------------------------------
	// ダメージを受ける処理を行う
	// 
	// ・引数
	// Damage：ダメージ量
	// ----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void TakeDamage(int32 Damage);

protected:
	// キャラクターのルートコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// キャラクターの行動状態
	UPROPERTY(EditAnywhere, Category = "State", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterActState E_CharacterActState;

	// ステータス
	UPROPERTY(EditAnywhere)
	FStatus CharacterStatus = FStatus();

	// 武器コンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<UChildActorComponent> EquippedWeaponComp;

	//// 装備中のウェポン
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<AWeaponBase> EquippedWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* BattleCameraSpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChildActorComponent* BattleCameraComp;

};