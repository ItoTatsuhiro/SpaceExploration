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
class UNiagaraComponent;

/**
 * 
 */
UCLASS(BlueprintType)
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

	// --------------------------------------------------------------------------
	// 死亡時の処理を行う
	// --------------------------------------------------------------------------
	void SetCharacterLocation(const FVector& Location) override;

	// -----------------------------------------------------------------
	// プレイヤーを見下ろしているカメラのコンポーネントを返す。
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	UChildActorComponent* GetLookingDownCaemeraComponent() const { return LookingDownCameraComp; }

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

	// -----------------------------------------------------------------
	// プレイヤーの経験値を追加。
	// 
	// args1...追加する経験値
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void AddExp(int Exp);

	// -----------------------------------------------------------------
	// レベルが上げられるか判定を返す。
	// 
	// args1...追加する経験値
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	bool CanLevelUp();

	// -----------------------------------------------------------------
	// プレイヤーのレベルアップを実行する
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void ExecuteLevelUp();

private:	
	// スタティックメッシュコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CharacterStaticMeshComp;

	// 噴射エフェクトのコンポーネント
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Naigara", meta = (AllowPrivateAccess = "true") )
	TObjectPtr<UNiagaraComponent> JetNiagaraComp;

	// プレイヤーの各レベルのステータスや必要経験値を定義したデータテーブル
	UPROPERTY(EditAnywhere, Category = "DataTable")
	TSoftObjectPtr<UDataTable> PlayerDataTable;

	// プレイヤーのシーケンス
	Sequence PlayerSequence;

	// 移動方向への速度
	UPROPERTY(EditAnywhere)
	FVector MoveVelocity;

	// 目的位置
	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	// プレイヤーの通常位置
	UPROPERTY(VisibleAnywhere)
	FVector DefaultPlayerLocation;

	// 通常のプレイヤー回転値
	UPROPERTY(VisibleAnywhere)
	FRotator DefaultPlayerRotate;

	// 通常の見下ろしカメラアームの回転値
	UPROPERTY(VisibleAnywhere)
	FRotator DefaultLookingDownCameraRotate;

	// 待機時のアングル
	UPROPERTY(VisibleAnywhere)
	int IdleAngle;

	// 移動速度
	UPROPERTY(EditAnywhere)
	float MoveSpeed;

	UPROPERTY(EditAnywhere)
	float DeathEffectTime;

	// 見下ろし方のカメラのスプリングアーム
	UPROPERTY(EditAnywhere, Category = "Components")
	USpringArmComponent* LookingDownCameraSpringArm;

	// 見下ろし方のカメラコンポーネント
	UPROPERTY(EditAnywhere, Category = "Components")
	UChildActorComponent* LookingDownCameraComp;
	
private:
	// -----------------------------------------------------------------
	// 指定したレベルのプレイヤーのステータスをセットする
	// 
	// SetLevel...セットするステータスのレベル
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void SetStatusForDataTable(int SetLevel);

	// -----------------------------------------------------------------
	// 左クリックを押したとき、インターフェースを実行する。
	// -----------------------------------------------------------------
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
