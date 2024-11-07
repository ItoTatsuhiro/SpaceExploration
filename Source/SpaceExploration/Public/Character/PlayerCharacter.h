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

	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UChildActorComponent* GetLookingDownCaemeraComponent() const { return LookingDownCameraComp; }

	UFUNCTION(BlueprintCallable)
	UWeaponInventoryComponent* GetWeaponInventoryComponent() { return WeaponInventoryComponent; }

	// 目標の位置に移動
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
};
