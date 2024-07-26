// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

private:
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
	
public:
	// 目標の位置に移動
	void MoveTargetLocation(const FVector Location);

private:
	// 待機シーケンス
	bool seqIdle(float DeltaTime);
	// 移動シーケンス
	bool seqMoveTargetLocation(float DeltaTime);
};
