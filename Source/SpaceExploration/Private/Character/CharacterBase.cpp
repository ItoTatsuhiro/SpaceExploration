// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "tsutsumi/Element.h"
#include "Weapon/WeaponBase.h"
// #include "tsutsumi/Status.h"

// Sets default values
ACharacterBase::ACharacterBase() : CharacterLocation( 0.0f, 0.0f, 0.0f )
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���[�g�R���|�[�l���g�̐ݒ�
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = DefaultSceneRoot;

	// �X�^�e�B�b�N���b�V���̐ݒ�
	CharacterMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	SetCharacterStaticMesh(TEXT("/Game/Mesh/Cube"));

	// StaticMeshComponent��RootComponent��Attach����
	CharacterMeshComp->SetupAttachment(RootComponent);

	 // �X�e�[�^�X�̏�����
	SetCharacterStatus(1.0f, 1.0f, 0.0f, 0.0f, 0.0f);

	// �����i������
	EquippedWeapon = nullptr;

}

ACharacterBase::~ACharacterBase()
{

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

const TObjectPtr<AWeaponBase> ACharacterBase::GetEquippedWeapon() const
{
	return EquippedWeapon;
}

// �X�e�[�^�X���Z�b�g����
void ACharacterBase::SetCharacterStatus(float MaxHP, float HP, float Attack, float Defence, float Speed)
{
	CharacterStatus.HP = HP;
	CharacterStatus.AttackPower = Attack;
	CharacterStatus.DefencePower = Defence;
	CharacterStatus.Speed = Speed;
}

void ACharacterBase::SetCharacterStaticMesh(const TCHAR* file_path)
{
	// StaticMesh��Laod����StaticMeshComponent��StaticMesh�ɐݒ肷��
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, file_path, NULL, LOAD_None, NULL);

	CharacterMeshComp->SetStaticMesh(Mesh);
}

void ACharacterBase::SetEquippedWeapon(TObjectPtr<AWeaponBase> Weapon)
{
	if (!Weapon) {
		UE_LOG(LogTemp, Display, TEXT("nullptr���Z�b�g����܂���"));
	}

	EquippedWeapon = Weapon;
}

