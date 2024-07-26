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
	// �X�^�e�B�b�N���b�V���R���|�[�l���g
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CharacterStaticMeshComp;

	Sequence PlayerSequence;

	// �ړ������ւ̑��x
	UPROPERTY(EditAnywhere)
	FVector MoveVelocity;

	// �ړI�ʒu
	UPROPERTY(EditAnywhere)
	FVector TargetLocation;

	// �ړ����x
	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	
public:
	// �ڕW�̈ʒu�Ɉړ�
	void MoveTargetLocation(const FVector Location);

private:
	// �ҋ@�V�[�P���X
	bool seqIdle(float DeltaTime);
	// �ړ��V�[�P���X
	bool seqMoveTargetLocation(float DeltaTime);
};
