// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "tsutsumi/Element.h"
#include "Weapon/WeaponBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include "tsutsumi/Status.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ルートコンポーネントの設定
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = DefaultSceneRoot;

	// 装備品初期化
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

// 装備する武器を設定する。
void ACharacterBase::SetEquippedWeapon(AWeaponBase* Weapon)
{
	if (!Weapon) {
		UE_LOG(LogClass, Display, TEXT("nullptrがセットされました"));
	}

	EquippedWeapon = Weapon;

	FString ResultLog = Weapon->GetWeaponStatus().PlayerName + " equipped";
	UKismetSystemLibrary::PrintString(this, ResultLog, true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("%s"), *ResultLog);
}

// =========================================================================
// キャラクターのHPを回復させて、回復量を返す関数
// 
// ・引数
// RecoveryAmount：回復する値（デフォルト値の場合、全回復する）
// 
// 戻り値：回復した値
// =========================================================================
int32 ACharacterBase::RecoverHP(int32 RecoveryAmount)
{
	int32 ResultAmount = RecoveryAmount;

	if (RecoveryAmount == -1) {
		ResultAmount = CharacterStatus.MaxHp - CharacterStatus.HP;
		CharacterStatus.HP = CharacterStatus.MaxHp;
		UKismetSystemLibrary::PrintString(this, "HP fully recovered.", true, true, FColor::Yellow, 2.f, TEXT(""));
		UE_LOG(LogTemp, Warning, TEXT("HP fully recovered."), nullptr);
		return ResultAmount;
	}
	CharacterStatus.HP += RecoveryAmount;

	if (CharacterStatus.HP > CharacterStatus.MaxHp) {
		ResultAmount -= CharacterStatus.HP - CharacterStatus.MaxHp;
		CharacterStatus.HP = CharacterStatus.MaxHp;
	}

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("HP recovered %d times."), ResultAmount), true, true, FColor::Yellow, 2.f, TEXT(""));
	UE_LOG(LogTemp, Warning, TEXT("HP recovered %d times."), ResultAmount);

	return ResultAmount;
}

// =========================================================================
// 攻撃行動を開始
// =========================================================================
void ACharacterBase::StartAttackAction()
{

	UKismetSystemLibrary::PrintString(this, "StartAttack", true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("攻撃開始"), nullptr);
}

// =========================================================================
// ダメージを受ける処理を行う
// 
// ・引数
// Damage：ダメージ量
// =========================================================================
void ACharacterBase::TakeDamage(int32 Damage)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%d damaged"), Damage), true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("%d damaged"), Damage);
	CharacterStatus.HP -= Damage;
}
