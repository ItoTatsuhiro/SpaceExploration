// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../tsutsumi/Status.h"
#include "../Weapon/WeaponBase.h"
#include "CharacterBase.generated.h"

UCLASS(Abstract)
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 攻撃関数
	// UFunction(BlueprintCallable)
	virtual void Attack() PURE_VIRTUAL(ACharacterBase::Attack, );

	// ----------------------------------------------------------------
	// ゲッター
	// ----------------------------------------------------------------

	// 生存しているか判定を返す
	inline bool IsAlive() const { return CharacterStatus.HP > 0; }
	// キャラクターステータスを取得する
	inline const FStatus& GetCharacterStatus() const { return CharacterStatus; }
	// キャラクターの位置を取得
	inline const FVector& GetCharacterLocation() const { return CharacterLocation; }
	// 装備中の武器を取得
	const TObjectPtr<AWeaponBase> GetEquippedWeapon() const { return EquippedWeapon; };

	// ----------------------------------------------------------------
	// セッター
	// ----------------------------------------------------------------

	// キャラクターの位置を設定する。
	inline void SetCharacterLocation(const FVector& Location) 
	{
		CharacterLocation = Location;
		GetOwner()->SetActorLocation(CharacterLocation);
	}
	// ステータスをセットする。
	inline void SetCharacterStatus(const FStatus& Status) { CharacterStatus = Status; }
	// 装備する武器を設定する。
	void SetEquippedWeapon(TObjectPtr<AWeaponBase> Weapon);

	// ----------------------------------------------------------------
	// その他
	// ----------------------------------------------------------------

	// キャラクターのHPを回復させて、回復量を返す関数
	// 引数：回復する値
	// 戻り値：回復した値
	int32 RecoverHP(int32 RecoveryAmount);

	// ダメージを受ける
	// 引数：ダメージ量
	void TakeDamage(int32 Damage);

protected:
	// キャラクターのルートコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// スタティックメッシュコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> CharacterMeshComp;

	// キャラクターの位置
	UPROPERTY(EditAnywhere)
	FVector CharacterLocation;

	// ステータス
	UPROPERTY(EditAnywhere)
	FStatus CharacterStatus;

	// 装備中のウェポン
	UPROPERTY(EditAnywhere)
	TObjectPtr<AWeaponBase> EquippedWeapon;

};