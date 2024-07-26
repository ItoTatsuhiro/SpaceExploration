// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../tsutsumi/Status.h"
#include "../Weapon/WeaponBase.h"
#include "CharacterBase.generated.h"

DECLARE_DELEGATE_RetVal_OneParam(bool, Sequence, float);

UCLASS(BlueprintType, Blueprintable, Abstract)
class SPACEEXPLORATION_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �U���֐�
	virtual void Attack() PURE_VIRTUAL(ACharacterBase::Attack, );

	// ----------------------------------------------------------------
	// �Q�b�^�[
	// ----------------------------------------------------------------

	// �������Ă��邩�����Ԃ�
	UFUNCTION(BlueprintCallable)
	inline bool IsAlive() const { return CharacterStatus.HP > 0; }
	// �L�����N�^�[�X�e�[�^�X���擾����
	UFUNCTION(BlueprintCallable)
	inline FStatus& GetCharacterStatus() { return CharacterStatus; }
	// �L�����N�^�[�̈ʒu���擾
	// �������̕�����擾
	UFUNCTION(BlueprintCallable)
	AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; };

	// ----------------------------------------------------------------
	// �Z�b�^�[
	// ----------------------------------------------------------------

	// �L�����N�^�[�̈ʒu��ݒ肷��B
	UFUNCTION(BlueprintCallable)
	inline void SetCharacterLocation(const FVector& Location) 
	{
		GetOwner()->SetActorLocation(Location);
	}

	// �X�e�[�^�X���Z�b�g����B
	UFUNCTION(BlueprintCallable)
	inline void SetCharacterStatus(FStatus Status) 
	{ 
		CharacterStatus = Status; 
	}
	
	// �������镐���ݒ肷��B
	UFUNCTION(BlueprintCallable)
	void SetEquippedWeapon(AWeaponBase* Weapon);

	// ----------------------------------------------------------------
	// ���̑�
	// ----------------------------------------------------------------

	// �L�����N�^�[��HP���񕜂����āA�񕜗ʂ�Ԃ��֐�
	// �����F�񕜂���l�i�f�t�H���g�l�̏ꍇ�A�S�񕜂���j
	// �߂�l�F�񕜂����l
	UFUNCTION(BlueprintCallable)
	int32 RecoverHP(int32 RecoveryAmount = -1);

	// �_���[�W���󂯂�
	// �����F�_���[�W��
	UFUNCTION(BlueprintCallable)
	void TakeDamage(int32 Damage);

protected:
	// �L�����N�^�[�̃��[�g�R���|�[�l���g
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// �X�e�[�^�X
	UPROPERTY(EditAnywhere)
	FStatus CharacterStatus;

	// �������̃E�F�|��
	UPROPERTY(EditAnywhere)
	TObjectPtr<AWeaponBase> EquippedWeapon;

};