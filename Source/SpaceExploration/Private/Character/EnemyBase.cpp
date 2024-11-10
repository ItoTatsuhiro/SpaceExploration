// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBase.h"
#include <Kismet/KismetSystemLibrary.h>

AEnemyBase::AEnemyBase() : EnemyElement(EElement::fire)
{
	CharacterMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("EnemyMesh");
	CharacterMeshComp->SetupAttachment(DefaultSceneRoot);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	Attack();
}

// 攻撃関数
void AEnemyBase::Attack()
{
	AWeaponBase* EquippedWeapon =  Cast<AWeaponBase>( EquippedWeaponComp->GetChildActor() );

	if (!EquippedWeapon) {
		return;
	}

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("AEnemyBase:Attack")), true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("Attack"));

	EquippedWeapon->ExecuteAttack();
}

// 待機行動シーケンス
bool AEnemyBase::SeqStandbyAction(const float delta_time)
{
	return true;
}

// 攻撃行動シーケンス
bool AEnemyBase::SeqAttackAction(const float delta_time)
{
	return false;
}
