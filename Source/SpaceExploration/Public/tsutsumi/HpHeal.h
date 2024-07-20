// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HpHeal.generated.h"

UCLASS()
class SPACEEXPLORATION_API AHpHeal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHpHeal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
