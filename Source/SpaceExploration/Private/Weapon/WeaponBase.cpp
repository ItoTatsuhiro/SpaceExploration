// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Character/PlayerCharacter.h"
#include "UObject/NoExportTypes.h"
#include "Math/RandomStream.h"
#include "random"
#include "Engine/LevelScriptActor.h"
// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.                             
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent                = CreateDefaultSubobject<USceneComponent>(TEXT("Default Component"));
	RootComponent                 = SceneComponent;
	MeshComponent                 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	MeshComponent                 -> SetupAttachment(RootComponent);
}
// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}
void AWeaponBase::SettingWeapon()
{
		UE_LOG(LogTemp, Warning, TEXT("Set"));
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (PlayerCharacter)
		{
			AWeaponBase* EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
			if (!EquippedWeapon)
			{
				NextLevelStatus = CurrentLevelStatus;
				UE_LOG(LogTemp, Error, TEXT("•Ší‚ª‹ó‚Å‚·"));
				return;
			}
				if (EquippedWeapon->WeaponElement == WeaponElement)
				{
					int level                    = CurrentLevelStatus.PlayerLevel + 1;
					NextLevelStatus.PlayerName   = CurrentLevelStatus.PlayerName;
					NextLevelStatus.PlayerLevel  = level;
					NextLevelStatus.HP           = CurrentLevelStatus.HP + ((level - 1) * HpEnhancedValue);
					NextLevelStatus.Speed        = CurrentLevelStatus.Speed + ((level - 1) * SpeedEnhancedValue);
					NextLevelStatus.AttackPower  = CurrentLevelStatus.AttackPower + ((level - 1) * AttackEnhancedValue);
					NextLevelStatus.DefencePower = CurrentLevelStatus.DefencePower + ((level - 1) * DeffenceEnhancedValue);
				}
				else
				{
					NextLevelStatus = CurrentLevelStatus;
				}
		}
		
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
	CurrentLevelStatus = NextLevelStatus;
	PlayerCharacter->SetEquippedWeapon(this);
	UWorld* World = GetWorld();
	ALevelScriptActor* LevelScript = World->GetLevelScriptActor();
	if (LevelScript)
	{
		FName FunctionName(TEXT("Weapon Sets")); 
		UFunction* Function = LevelScript->FindFunction(FunctionName);

		if (Function)
		{
			LevelScript->ProcessEvent(Function, nullptr);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("WeaponSets function not found"));
		}
	}
	Destroy();
}