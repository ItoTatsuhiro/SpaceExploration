// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "tsutsumi/Element.h"
#include "Weapon/WeaponBase.h"
#include "tsutsumi/Status.h"

// Sets default values
ACharacterBase::ACharacterBase() : CharacterLocation(0.0f, 0.0f, 0.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���[�g�R���|�[�l���g�̐ݒ�
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = DefaultSceneRoot;

	// �X�^�e�B�b�N���b�V���̐ݒ�
	CharacterMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	// StaticMeshComponent��RootComponent��Attach����
	CharacterMeshComp->SetupAttachment(RootComponent);

	// �X�e�[�^�X�̏�����
	CharacterStatus.PlayerName = "No Name";
	CharacterStatus.MaxHp = 1.0f;
	CharacterStatus.HP = 1.0f;
	CharacterStatus.AttackPower = 1.0f;
	CharacterStatus.DefencePower = 1.0f;
	CharacterStatus.Speed = 1.0f;

	// �����i������
	EquippedWeapon = nullptr;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// �������镐���ݒ肷��B
void ACharacterBase::SetEquippedWeapon(TObjectPtr<AWeaponBase> Weapon)
{
	if (!Weapon) {
		UE_LOG(LogClass, Display, TEXT("nullptr���Z�b�g����܂���"));
	}

	EquippedWeapon = Weapon;
}

int32 ACharacterBase::RecoverHP(int32 RecoveryAmount)
{
	int32 ResultAmount = RecoveryAmount;
	CharacterStatus.HP += RecoveryAmount;

	if (CharacterStatus.HP > CharacterStatus.MaxHp) {
		ResultAmount -= CharacterStatus.HP - CharacterStatus.MaxHp;
		CharacterStatus.HP = CharacterStatus.MaxHp;
	}

	return ResultAmount;
}

void ACharacterBase::TakeDamage(int32 Damage)
{
	CharacterStatus.HP -= Damage;
}
