// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon1.h"

AWeapon1::AWeapon1()
{

	WeaponStatus.PlayerName = "Weapon01";
	WeaponStatus.MaxHp = 1.0f;
	WeaponStatus.HP = 1.0f;
	WeaponStatus.AttackPower = 1.0f;
	WeaponStatus.DefencePower = 1.0f;
	WeaponStatus.Speed = 1.0f;
}

void AWeapon1::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
