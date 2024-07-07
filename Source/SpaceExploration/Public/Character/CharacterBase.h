// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../tsutsumi/CPPCalcInfo.h"
#include "../Weapon/WeaponBase.h"
#include "CharacterBase.generated.h"

UCLASS(Abstract)
class SPACEEXPLORATION_API ACharacterBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterBase();
	virtual ~ACharacterBase();

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
	virtual void TakeDamage(int Damage) PURE_VIRTUAL( ACharacterBase::TakeDamage,);

	// ----------------------------------------------------------------
	// �Q�b�^�[
	// ----------------------------------------------------------------
	
	// �L�����N�^�[�X�e�[�^�X���擾����
	inline const FStatus& GetCharacterStatus() const { return CharacterStatus; }
	// �L�����N�^�[�̈ʒu���擾
	inline const FVector3f& GetCharacterLocation() const { return CharacterLocation; }
	// �������̕�����擾
	const TObjectPtr<AWeaponBase> GetEquippedWeapon() const;

	// ----------------------------------------------------------------
	// �Z�b�^�[
	// ----------------------------------------------------------------

	// �X�e�[�^�X���Z�b�g����B
	void SetCharacterStatus(float MaxHP, float HP, float Attack, float Defence, float Speed);
	// �L�����N�^�[�̃��b�V����ݒ肷��B
	void SetCharacterStaticMesh(const TCHAR* file_path);
	// �������镐���ݒ肷��B
	void SetEquippedWeapon(TObjectPtr<AWeaponBase> Weapon);

protected:
	// �L�����N�^�[�̃��[�g�R���|�[�l���g
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// �X�^�e�B�b�N���b�V���R���|�[�l���g
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CharacterMeshComp;

	// �L�����N�^�[�̈ʒu
	UPROPERTY(EditAnywhere)
	FVector3f CharacterLocation;

	// �X�e�[�^�X
	UPROPERTY(EditAnywhere)
	FStatus CharacterStatus;

	// �������̃E�F�|��
	UPROPERTY(EditAnywhere)
	TObjectPtr<AWeaponBase> EquippedWeapon;

};
