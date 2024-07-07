// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	CharacterStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerStaticMeshComponent"));

	CharacterStaticMeshComp->SetupAttachment(RootComponent);

}
