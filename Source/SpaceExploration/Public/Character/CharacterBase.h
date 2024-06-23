// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterBase.generated.h"

UCLASS(Abstract)
class SPACEEXPLORATION_API ACharacterBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �_���[�W���󂯂�
	virtual void Move(int Damage) PURE_VIRTUAL(ACharacterBase::Move, );

	// �U���֐�
	// UFunction(BlueprintCallable)
	virtual void Attack() PURE_VIRTUAL( ACharacterBase::Attack,);

	// �񕜊֐�
	virtual void RecoverHP(int RecoveryAmount) PURE_VIRTUAL( ACharacterBase::RecoverHP, );

	// �_���[�W���󂯂�
	virtual void TakeDamage(int Damage) PURE_VIRTUAL( ACharacterBase::TakeDamage, );

private:
	// �L�����N�^�[�̈ʒu
	UPROPERTY(EditAnywhere)
	FVector3f CharacterLocation;

	// �X�e�[�^�X

	 // �����i

};
