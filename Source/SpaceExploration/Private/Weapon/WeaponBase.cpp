// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase() : WeaponLocation(0.0f, 0.0f, 0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponStatus.HP = 1.0f;
	WeaponStatus.AttackPower = 0.0f;
	WeaponStatus.DefencePower = 0.0f;
	WeaponStatus.Speed = 0.0f;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 武器のステータスを設定する
void AWeaponBase::SetWeaponStatus(const FStatus& Status) {
	WeaponStatus.HP				= Status.HP;
	WeaponStatus.AttackPower	= Status.AttackPower;
	WeaponStatus.DefencePower	= Status.DefencePower;
	WeaponStatus.Speed			= Status.Speed;
}

