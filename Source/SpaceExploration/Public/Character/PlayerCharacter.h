// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnhancedActionKeyMapping.h"
#include "MouseButtonEvent.h"
#include "../Weapon/WeaponInventoryComponent.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UUserWidget;

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

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

	// -----------------------------------------------------------------
	// プレイヤーを見下ろしているカメラのコンポーネントを返す。
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	UChildActorComponent* GetLookingDownCaemeraComponent() const { return LookingDownCameraComp; }

	// -----------------------------------------------------------------
	// プレイヤーの武器インベントリのコンポーネントを返す。
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	UWeaponInventoryComponent* GetWeaponInventoryComponent() { return WeaponInventoryComponent; }

	// -----------------------------------------------------------------
	// 指定した属性を持つ武器のアドレスを返す。
	// 
	// args1...取得したい武器の属性
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	AWeaponBase* GetElementWeapon(EElement WeaponElement);

	// -----------------------------------------------------------------
	// 目標の位置にプレイヤーを移動させる
	// 
	// args1...プレイヤーを移動させたい位置
	// -----------------------------------------------------------------
	void BeginMoveTargetLocation(const FVector& Location);

private:	
	// スタティックメッシュコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CharacterStaticMeshComp;

	// プレイヤーのシーケンス
	Sequence PlayerSequence;

	// 移動方向への速度
	UPROPERTY(EditAnywhere)
	FVector MoveVelocity;

	// 目的位置
	UPROPERTY(EditAnywhere)
	FVector TargetLocation;

	// 移動速度
	UPROPERTY(EditAnywhere)
	float MoveSpeed;

	// 見下ろし方のカメラのスプリングアーム
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* LookingDownCameraSpringArm;

	// 見下ろし方のカメラコンポーネント
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChildActorComponent* LookingDownCameraComp;

	// 武器のインベントリコンポーネント
	UPROPERTY(VisibleAnywhere, Category = "Ineventory", BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UWeaponInventoryComponent* WeaponInventoryComponent;
	
private:
	// 左クリックを押したとき、インターフェースを実行する。
	void ClickedMouseLeftButton();

	// 待機シーケンス
	bool SeqIdle(const float DeltaTime);
	// 移動シーケンス
	bool SeqMoveTargetLocation(const float DeltaTime);
	// 攻撃シーケンス
	bool SeqAttack(const float DeltaTime);
	// ダメージを受けるシーケンス
	bool SeqTakeDamage(const float DelataTime);
	// 死亡演出を行うシーケンス
	bool SeqDeath(const float DeltaTime);

};
