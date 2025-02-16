// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "tsutsumi/Element.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/BaseCamera.h"
#include "Weapon/WeaponBase.h"
#include "NiagaraComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "tsutsumi/Status.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	UE_LOG(LogClass, Log, TEXT("ACharacterBaseのコンストラクタが実行されました。"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ルートコンポーネントの設定
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	// 武器の生成ポイントコンポーネントの設定
	WeaponSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponPoint"));
	WeaponSpawnPoint->SetupAttachment(DefaultSceneRoot);

	// バトルシーンのカメラ設定
	BattleCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("BattleCameraSpringArm"));
	BattleCameraSpringArm->SetupAttachment(DefaultSceneRoot);

	BattleCameraSpringArm->TargetArmLength = 2000.f;
	BattleCameraSpringArm->SetWorldRotation(FRotator(0.f, 0.f, 0.f));

	BattleCameraComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("BattleCamera"));
	BattleCameraComp->SetChildActorClass(ABaseCamera::StaticClass());
	BattleCameraComp->SetupAttachment(BattleCameraSpringArm);

	BattleCameraComp->SetWorldRotation(FRotator(0.f, 0.f, 180.f));

	// ナイアガラの設定
	DamageNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DamageNiagara"));
	DamageNiagaraComp->SetupAttachment(DefaultSceneRoot);
	DamageNiagaraComp->SetAutoActivate(false);

	DeathNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DeathNiagara"));
	DeathNiagaraComp->SetupAttachment(DefaultSceneRoot);
	DeathNiagaraComp->SetAutoActivate(false);

	// その他
	CharacterStatus = FStatus();

	E_CharacterActState = ECharacterActState::Idle;

	EquippedWeapon = nullptr;

	SequenceElapsedTime = 0.0f;
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

// --------------------------------------------------------------------------
// 装備中の武器を返す
// --------------------------------------------------------------------------
AWeaponBase* ACharacterBase::GetEquippedWeapon() const
{
	if (!EquippedWeapon)
	{
		UE_LOG(LogClass, Error, TEXT("ACharaterBase::GetEquippedWeapon() EquippedWeaponが nullptr でした。"));
		return nullptr;
	}

	return EquippedWeapon;
}

EElement ACharacterBase::GetAttackElement() const
{
	if (!EquippedWeapon)
	{
		return EElement::none;
	}

	return EquippedWeapon->GetWeaponElement();
}

// --------------------------------------------------------------------------
// 装備する武器を設定する。
// 
// Weapon...装備する武器
// --------------------------------------------------------------------------
void ACharacterBase::SetEquippedWeapon(AWeaponBase* Weapon)
{
	if (!Weapon) {
		UE_LOG(LogClass, Display, TEXT("nullptrがセットされました"));
		return;
	}

	if (EquippedWeapon) {
		// 装備していた武器を削除
		EquippedWeapon->Destroy();
	}

	// 装備する武器をクラスにアタッチし、装備中の武器に設定する。
	Weapon->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	EquippedWeapon = Weapon;
	EquippedWeapon->SetActorRelativeTransform(WeaponSpawnPoint->GetRelativeTransform());

	// 衝突判定を "No Collision" に設定する。
	UPrimitiveComponent* PrimitiveComp = EquippedWeapon->FindComponentByClass<UPrimitiveComponent>();

	if (PrimitiveComp)
	{
		PrimitiveComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("ACharcterBase::SetEquippedWeapon() : PrimitiveComponentの取得に失敗しました"));
	}

	FString ResultLog = Weapon->GetWeaponStatus().PlayerName + " equipped";

	UE_LOG(LogTemp, Log, TEXT("%s"), *ResultLog);
}

// --------------------------------------------------------------------------
// キャラクターのHPを回復させて、回復量を返す関数
// 
// ・引数
// RecoveryAmount：回復する値（デフォルト値の場合、全回復する）
// 
// 戻り値：回復した値
// --------------------------------------------------------------------------
int32 ACharacterBase::RecoverHP(int32 RecoveryAmount)
{
	int32 ResultAmount = RecoveryAmount;

	// -1 の場合、HPを全回復する。
	if (RecoveryAmount == -1) {
		ResultAmount = CharacterStatus.MaxHp - CharacterStatus.HP;
		CharacterStatus.HP = CharacterStatus.MaxHp;
		UKismetSystemLibrary::PrintString(this, "HP fully recovered.", true, true, FColor::Yellow, 2.f, TEXT(""));
		UE_LOG(LogTemp, Warning, TEXT("HP fully recovered."));
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

// --------------------------------------------------------------------------
// 攻撃行動を開始
// --------------------------------------------------------------------------
void ACharacterBase::StartAttackAction()
{

	UKismetSystemLibrary::PrintString(this, "StartAttack", true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("攻撃開始"));
}
