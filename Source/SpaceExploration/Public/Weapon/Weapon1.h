// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Weapon1.generated.h"

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API AWeapon1 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeapon1();

private:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
