// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "../tsutsumi/Element.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPACEEXPLORATION_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyBase();

protected:
	// �X�P���^�����b�V���R���|�[�l���g
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> CharacterMeshComp;

	// �G�̑���
	UPROPERTY(EditAnywhere)
	EElement EnemyElement;

};