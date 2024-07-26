// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() : TargetLocation({ 0, 0, 0 }), MoveSpeed(10.0f)
{
	CharacterStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayeMesh"));

	CharacterStaticMeshComp->SetupAttachment(RootComponent);

	PlayerSequence.BindUObject(this, &APlayerCharacter::seqIdle);

}

void APlayerCharacter::MoveTargetLocation(const FVector Location)
{
	TargetLocation = Location;
	PlayerSequence.BindUObject(this, &APlayerCharacter::seqMoveTargetLocation);
}

bool APlayerCharacter::seqIdle(float DeltaTime)
{
	return true;
}

bool APlayerCharacter::seqMoveTargetLocation(float DeltaTime)
{

	FVector MoveDirection =  (TargetLocation - GetActorLocation()).GetSafeNormal();
	
	AddMovementInput(MoveDirection);

	if (FVector::Dist(GetActorLocation(), TargetLocation) <= 100.f) {
		PlayerSequence.BindUObject(this, &APlayerCharacter::seqIdle);
	}
	return true;
}


