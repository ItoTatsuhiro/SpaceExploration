// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnhancedActionKeyMapping.h"
#include "MouseButtonEvent.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UCameraComponent;

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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetCaemraComponent() { return Camera; }

	// 目標の位置に移動
	void MoveTargetLocation(const FVector Location);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* ClickedEvnet;
	
	// スタティックメッシュコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CharacterStaticMeshComp;

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
	
private:
	//
	void ClickedMouseLeftButton();


	// 待機シーケンス
	bool seqIdle(float DeltaTime);
	// 移動シーケンス
	bool seqMoveTargetLocation(float DeltaTime);
};
