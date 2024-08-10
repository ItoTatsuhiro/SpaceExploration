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

	// ステータスの初期化
	CharacterStatus.PlayerName = "No Name";
	CharacterStatus.MaxHp = 1.0f;
	CharacterStatus.HP = 1.0f;
	CharacterStatus.AttackPower = 1.0f;
	CharacterStatus.DefencePower = 1.0f;
	CharacterStatus.Speed = 1.0f;

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

	FString ResultLog = EquippedWeapon->GetWeaponStatus().PlayerName + "を装備しました";
	UKismetSystemLibrary::PrintString(this, ResultLog, true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("%s"), *ResultLog);
}

int32 ACharacterBase::RecoverHP(int32 RecoveryAmount)
{
	int32 ResultAmount = RecoveryAmount;

	if (RecoveryAmount == -1) {
		ResultAmount = CharacterStatus.MaxHp - CharacterStatus.HP;
		CharacterStatus.HP = CharacterStatus.MaxHp;
		UKismetSystemLibrary::PrintString(this, "HPを全回復しました", true, true, FColor::Yellow, 2.f, TEXT(""));
		UE_LOG(LogTemp, Warning, TEXT("HPを全回復しました"), nullptr);
		return ResultAmount;
	}
	CharacterStatus.HP += RecoveryAmount;

	if (CharacterStatus.HP > CharacterStatus.MaxHp) {
		ResultAmount -= CharacterStatus.HP - CharacterStatus.MaxHp;
		CharacterStatus.HP = CharacterStatus.MaxHp;
	}

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("HPを%d回復しました"), ResultAmount), true, true, FColor::Yellow, 2.f, TEXT(""));
	UE_LOG(LogTemp, Warning, TEXT("HPを%d回復しました"), ResultAmount);

	return ResultAmount;
}

void ACharacterBase::StartAttackAction()
{

	UKismetSystemLibrary::PrintString(this, "StartAttack", true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("攻撃開始"), nullptr);
}

void ACharacterBase::TakeDamage(int32 Damage)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%dダメージ受けた"), Damage), true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("攻撃開始"), nullptr);
	CharacterStatus.HP -= Damage;
}
