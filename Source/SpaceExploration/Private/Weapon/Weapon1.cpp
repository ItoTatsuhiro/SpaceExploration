// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon1.h"

AWeapon1::AWeapon1()
{

	CurrentLevelStatus.PlayerName = "Weapon01";
	CurrentLevelStatus.MaxHp = 1.0f;
	CurrentLevelStatus.HP = 1.0f;
	CurrentLevelStatus.AttackPower = 1.0f;
	CurrentLevelStatus.DefencePower = 1.0f;
	CurrentLevelStatus.Speed = 1.0f;
}

void AWeapon1::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
