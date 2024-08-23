// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Character/PlayerCharacter.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Component"));
	RootComponent = SceneComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	MeshComponent->SetupAttachment(RootComponent);

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

void AWeaponBase::LeftMouseButtonEvent_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) {
		UKismetSystemLibrary::PrintString(this, "PlayerCharacter is nullptr", true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is nullptr"), nullptr);
		return;
	}

	PlayerCharacter->SetEquippedWeapon(this);
	Destroy();
}


