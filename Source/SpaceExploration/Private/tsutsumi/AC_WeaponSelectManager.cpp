// Fill out your copyright notice in the Description page of Project Settings.


#include "tsutsumi/AC_WeaponSelectManager.h"
#include "Weapon\WeaponBase.h"
#include "Character\PlayerCharacter.h"
#include "tsutsumi\Status.h"
#include "Kismet/GameplayStatics.h"
#include "Manager\PlaySceneGameModeBase.h"

// Sets default values
AAC_WeaponSelectManager::AAC_WeaponSelectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAC_WeaponSelectManager::BeginPlay()
{
	Super::BeginPlay();

	OnLevelUpDelegate.BindUObject(this, &AAC_WeaponSelectManager::WeaponLevelUp);
	
}

// Called every frame
void AAC_WeaponSelectManager::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
	
	if (isClickInput_) {
		UE_LOG(LogTemp, Error, TEXT("true"));
		OnLevelUpDelegate.Execute();
	}

	// “ü—Í‚Ìó‘Ô‚ðŠm”F
	CheckClickInput();
}



void AAC_WeaponSelectManager::WeaponLevelUp()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) 
	{
		return;
	}
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(PerformRaycast());
	if (!WeaponBase)
	{
		return;
	}
	APlayerCharacter* Player = playerCharacter_;
	if (!Player)return;
	AWeaponBase* SameWeapon = Player->GetElementWeapon(WeaponBase->GetWeaponElement());
	if(!SameWeapon)
	{
		return;
	}
	SameWeapon->SetWeaponStatus(WeaponBase->GetWeaponNextLevelStatus());
	APlaySceneGameModeBase* PlaySceneGameMode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!PlaySceneGameMode)return;
	OnLevelUpDelegate.BindUObject(this, &AAC_WeaponSelectManager::MapLevelChange);
	UE_LOG(LogTemp, Error, TEXT("aaa"));
	PlaySceneGameMode->ChangeLevel(World,this);
}

void AAC_WeaponSelectManager::MapLevelChange()
{
}

