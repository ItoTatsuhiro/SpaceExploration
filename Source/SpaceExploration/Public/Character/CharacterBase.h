// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../tsutsumi/CPPCalcInfo.h"
#include "../Weapon/WeaponBase.h"
#include "CharacterBase.generated.h"

UCLASS(Abstract)
class SPACEEXPLORATION_API ACharacterBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterBase();
	virtual ~ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ダメージを受ける
	virtual void Move(int Damage) PURE_VIRTUAL(ACharacterBase::Move, );

	// 攻撃関数
	// UFunction(BlueprintCallable)
	virtual void Attack() PURE_VIRTUAL( ACharacterBase::Attack,);

	// 回復関数
	virtual void RecoverHP(int RecoveryAmount) PURE_VIRTUAL( ACharacterBase::RecoverHP, );

	// ダメージを受ける
	virtual void TakeDamage(int Damage) PURE_VIRTUAL( ACharacterBase::TakeDamage,);

	// ----------------------------------------------------------------
	// ゲッター
	// ----------------------------------------------------------------
	
	// キャラクターステータスを取得する
	inline const FStatus& GetCharacterStatus() const { return CharacterStatus; }
	// キャラクターの位置を取得
	inline const FVector3f& GetCharacterLocation() const { return CharacterLocation; }
	// 装備中の武器を取得
	const TObjectPtr<AWeaponBase> GetEquippedWeapon() const;

	// ----------------------------------------------------------------
	// セッター
	// ----------------------------------------------------------------

	// ステータスをセットする。
	void SetCharacterStatus(float MaxHP, float HP, float Attack, float Defence, float Speed);
	// キャラクターのメッシュを設定する。
	void SetCharacterStaticMesh(const TCHAR* file_path);
	// 装備する武器を設定する。
	void SetEquippedWeapon(TObjectPtr<AWeaponBase> Weapon);

protected:
	// キャラクターのルートコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// スタティックメッシュコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CharacterMeshComp;

	// キャラクターの位置
	UPROPERTY(EditAnywhere)
	FVector3f CharacterLocation;

	// ステータス
	UPROPERTY(EditAnywhere)
	FStatus CharacterStatus;

	// 装備中のウェポン
	UPROPERTY(EditAnywhere)
	TObjectPtr<AWeaponBase> EquippedWeapon;

};
