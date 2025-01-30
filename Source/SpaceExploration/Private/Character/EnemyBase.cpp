// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBase.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterface.h"
#include <Kismet/KismetSystemLibrary.h>

AEnemyBase::AEnemyBase() : EnemyElement(EElement::fire)
{
	CharacterMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("EnemyMesh");
	CharacterMeshComp->SetupAttachment(DefaultSceneRoot);

	EnemyActionSequence.BindUObject(this, &AEnemyBase::SeqStandbyAction);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnemyActionSequence.Execute(DeltaTime);
}

// --------------------------------------------------------------------------
// 攻撃処理を行う
// --------------------------------------------------------------------------
void AEnemyBase::Attack()
{
	if (!EquippedWeapon) {
		UE_LOG(LogClass, Error, TEXT("AEnemyBase::Attack() : error : EquippedWeapon is nullptr"));
		return;
	}

	UE_LOG(LogClass, Log, TEXT("AEnemyBase::Attack()"));

	EnemyActionSequence.BindUObject(this, &AEnemyBase::SeqAttackAction);
	E_CharacterActState = ECharacterActState::Attack;
	EquippedWeapon->ExecuteAttack();
}

// --------------------------------------------------------------------------
// ダメージを受ける処理を行う
// 
// ・引数
// Damage：ダメージ量
// --------------------------------------------------------------------------
void AEnemyBase::TakeDamage(int32 Damage)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%sは%dダメージ受けた"), *( CharacterStatus.PlayerName ), Damage), true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("AEnemyBase::TakeDamage() : %sは%dダメージ受けた"), *(CharacterStatus.PlayerName), Damage);
	CharacterStatus.HP -= Damage;
	E_CharacterActState = ECharacterActState::TakeDamage;

	SequenceElapsedTime = 0.0f;
	EnemyActionSequence.BindUObject(this, &AEnemyBase::SeqTakeDamage);
	DamageNiagaraComp->Activate();
}

// --------------------------------------------------------------------------
// 死亡時の処理を行う
// --------------------------------------------------------------------------
void AEnemyBase::Death()
{
	DeathNiagaraComp->Activate();
	SequenceElapsedTime = 0.0f;
	E_CharacterActState = ECharacterActState::Death;
	EnemyActionSequence.BindUObject(this, &AEnemyBase::SeqDeath);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%sはやられた"), *(CharacterStatus.PlayerName)), true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("AEnemyBase::Death() : %sはやられた"), *(CharacterStatus.PlayerName));
}

// --------------------------------------------------------------------------
// 待機行動シーケンス
// --------------------------------------------------------------------------
bool AEnemyBase::SeqStandbyAction(const float delta_time)
{
	return true;
}

// --------------------------------------------------------------------------
// 攻撃行動シーケンス
// --------------------------------------------------------------------------
bool AEnemyBase::SeqAttackAction(const float delta_time)
{

	if (!EquippedWeapon)
	{
		UE_LOG(LogClass, Log, TEXT("EquippedWeapon is nullptr"));
		E_CharacterActState = ECharacterActState::Idle;
		EnemyActionSequence.BindUObject(this, &AEnemyBase::SeqStandbyAction);
		return true;
	}

	if (!EquippedWeapon->IsAttack())
	{
		UE_LOG(LogClass, Log, TEXT("WeaponAttack is not Active"));
		E_CharacterActState = ECharacterActState::Idle;
		EnemyActionSequence.BindUObject(this, &AEnemyBase::SeqStandbyAction);
	}

	return true;
}

// --------------------------------------------------------------------------
// ダメージを受けるシーケンス
// --------------------------------------------------------------------------
bool AEnemyBase::SeqTakeDamage(const float DeltaTime)
{
	if (DamageNiagaraComp->IsActive())
	{
		return true;
	}

	// HPが 0 の場合、死亡演出を行う
	if (CharacterStatus.HP <= 0)
	{
		Death();
		return true;
	}

	UE_LOG(LogClass, Log, TEXT("ダメージ演出終了"));
	E_CharacterActState = ECharacterActState::Idle;
	EnemyActionSequence.BindUObject(this, &AEnemyBase::SeqStandbyAction);

	return true;
}

// --------------------------------------------------------------------------
// 死亡の演出を行うシーケンス
// --------------------------------------------------------------------------
bool AEnemyBase::SeqDeath(const float DeltaTime)
{
	if (DeathNiagaraComp->IsActive())
	{
		return true;
	}

	UE_LOG(LogClass, Log, TEXT("死亡時の演出終了"));
	E_CharacterActState = ECharacterActState::Idle;
	EnemyActionSequence.BindUObject(this, &AEnemyBase::SeqStandbyAction);

	return true;
}
