// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../tsutsumi/Status.h"
#include "../tsutsumi/Element.h"
#include "../Character/MouseButtonEvent.h"
#include "WeaponBase.generated.h"


class UNiagaraComponent;

UCLASS(Abstract, Blueprintable)
class SPACEEXPLORATION_API AWeaponBase : public AActor, public IMouseButtonEvent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 武器のステータスを取得する
	UFUNCTION(BlueprintCallable)
	inline FStatus GetWeaponStatus() const { return CurrentLevelStatus; }
	inline FStatus GetWeaponNextLevelStatus() const { return NextLevelStatus; }
	// 武器の属性を取得する
	inline const EElement GetWeaponElement() const { return WeaponElement; }

	// ==========================================================================
	// ここから
	// ==========================================================================
	// 武器のステータスを設定する
	inline void SetWeaponStatus(const FStatus& Status) { CurrentLevelStatus = Status; }

	// 左マウスをクリックしたときの処理
	virtual void LeftMouseButtonEvent_Implementation(APlayerCharacter* PlayerCharacter) override;

	UFUNCTION(BlueprintCallable)
	void SettingWeapon();

	UFUNCTION(BlueprintCallable)
	void ExecuteAttack();

	UFUNCTION(BlueprintCallable)
	bool IsAttack();

protected:
	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComponent;

	// 武器のメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	// 武器の攻撃エフェクト
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> WeaponAttackNiagaraComp;

	//武器の初期ステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FStatus CurrentLevelStatus;

	//武器の次のレベルのステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FStatus NextLevelStatus;

	// 武器の属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	EElement WeaponElement = EElement::fire;

	//HPのレベルごとの強化値
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "EnhancedValue")
	int HpEnhancedValue;

	//スピードのレベルごとの強化値
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "EnhancedValue")
	int SpeedEnhancedValue;

	//攻撃力のレベルごとの強化値
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "EnhancedValue")
	int AttackEnhancedValue;

	//防御力のレベルごとの強化値
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "EnhancedValue")
	int DeffenceEnhancedValue;
};
