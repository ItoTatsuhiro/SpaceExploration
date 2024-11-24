// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Scene/AC_SceneManagerBase.h"
#include "AC_WeaponSelectManager.generated.h"

DECLARE_DELEGATE(FWeaponSelectDelegate);
UCLASS()


class SPACEEXPLORATION_API AAC_WeaponSelectManager : public AAC_SceneManagerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_WeaponSelectManager();
	FWeaponSelectDelegate OnLevelUpDelegate;
	void WeaponLevelUp();
	void MapLevelChange();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere);
	UWorld* World;
};
