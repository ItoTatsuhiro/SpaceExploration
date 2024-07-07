// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

private:
	// スタティックメッシュコンポーネント
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CharacterStaticMeshComp;
	
};
