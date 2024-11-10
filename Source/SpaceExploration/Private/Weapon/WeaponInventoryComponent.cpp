// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponInventoryComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "Weapon/WeaponBase.h"

// Sets default values for this component's properties
UWeaponInventoryComponent::UWeaponInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ElementWeapons.Init(nullptr, 3);

	ElementWeaponsClass.Init(nullptr, 3);

}


// Called when the game starts
void UWeaponInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UWeaponInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponInventoryComponent::CreateElememtWeapons()
{
	for (int32 i = 0; i < ElementWeaponsClass.Num(); ++i)
	{
		if (!ElementWeaponsClass[i]) {
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ElemenetWeaponのTSubclassOfが空です。")), true, true, FColor::Red, 2.f, TEXT(""));
			UE_LOG(LogTemp, Error, TEXT("ElemenetWeaponのTSubclassOfが空です。"));
			continue;
		}

		auto SpawnActor = GetWorld()->SpawnActor<AWeaponBase>(ElementWeaponsClass[i]);
		
		if (!SpawnActor) {
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("武器の生成を失敗しました。")), true, true, FColor::Red, 2.f, TEXT(""));
			UE_LOG(LogTemp, Error, TEXT("武器の生成を失敗しました。"));
			continue;
		}
		ElementWeapons[i] = SpawnActor;
		ElementWeapons[i]->SetActorHiddenInGame(true);
	}
}

