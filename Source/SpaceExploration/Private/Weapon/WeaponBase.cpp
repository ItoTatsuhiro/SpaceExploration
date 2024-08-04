// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Component"));
	RootComponent = SceneComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	MeshComponent->SetupAttachment(RootComponent);

	WeaponStatus.PlayerName = "No Name";
	WeaponStatus.MaxHp = 1.0f;
	WeaponStatus.HP = 1.0f;
	WeaponStatus.AttackPower = 0.0f;
	WeaponStatus.DefencePower = 0.0f;
	WeaponStatus.Speed = 0.0f;

	WeaponElement = EElement::fire;

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
