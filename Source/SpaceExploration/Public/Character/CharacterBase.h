// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../tsutsumi/Status.h"
#include "../Weapon/WeaponBase.h"
#include "CharacterBase.generated.h"

UCLASS(Abstract)
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �U���֐�
	// UFunction(BlueprintCallable)
	virtual void Attack() PURE_VIRTUAL(ACharacterBase::Attack, );

	// ----------------------------------------------------------------
	// �Q�b�^�[
	// ----------------------------------------------------------------

	// �������Ă��邩�����Ԃ�
	inline bool IsAlive() const { return CharacterStatus.HP > 0; }
	// �L�����N�^�[�X�e�[�^�X���擾����
	inline const FStatus& GetCharacterStatus() const { return CharacterStatus; }
	// �L�����N�^�[�̈ʒu���擾
	inline const FVector& GetCharacterLocation() const { return CharacterLocation; }
	// �������̕�����擾
	const TObjectPtr<AWeaponBase> GetEquippedWeapon() const { return EquippedWeapon; };

	// ----------------------------------------------------------------
	// �Z�b�^�[
	// ----------------------------------------------------------------

	// �L�����N�^�[�̈ʒu��ݒ肷��B
	inline void SetCharacterLocation(const FVector& Location) 
	{
		CharacterLocation = Location;
		GetOwner()->SetActorLocation(CharacterLocation);
	}
	// �X�e�[�^�X���Z�b�g����B
	inline void SetCharacterStatus(const FStatus& Status) { CharacterStatus = Status; }
	// �������镐���ݒ肷��B
	void SetEquippedWeapon(TObjectPtr<AWeaponBase> Weapon);

	// ----------------------------------------------------------------
	// ���̑�
	// ----------------------------------------------------------------

	// �L�����N�^�[��HP���񕜂����āA�񕜗ʂ�Ԃ��֐�
	// �����F�񕜂���l
	// �߂�l�F�񕜂����l
	int32 RecoverHP(int32 RecoveryAmount);

	// �_���[�W���󂯂�
	// �����F�_���[�W��
	void TakeDamage(int32 Damage);

protected:
	// �L�����N�^�[�̃��[�g�R���|�[�l���g
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// �X�^�e�B�b�N���b�V���R���|�[�l���g
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> CharacterMeshComp;

	// �L�����N�^�[�̈ʒu
	UPROPERTY(EditAnywhere)
	FVector CharacterLocation;

	// �X�e�[�^�X
	UPROPERTY(EditAnywhere)
	FStatus CharacterStatus;

	// �������̃E�F�|��
	UPROPERTY(EditAnywhere)
	TObjectPtr<AWeaponBase> EquippedWeapon;

};