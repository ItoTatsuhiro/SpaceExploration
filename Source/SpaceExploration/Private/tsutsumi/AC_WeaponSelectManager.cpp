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
	Super::Tick(DeltaTime);

}
void AAC_WeaponSelectManager::WeaponLevelUp()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)return;
	bool Hit = false;
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility),Hit,HitResult);
	if (!Hit)return;
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(HitResult.GetActor());
	if (!WeaponBase)return;
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!Player)return;
	AWeaponBase* PlayerWeapon = Player->GetEquippedWeapon();
	if (WeaponBase->GetWeaponElement() == PlayerWeapon->GetWeaponElement()) {
		FStatus NewStatus = WeaponBase->GetWeaponStatus();
		NewStatus.PlayerLevel += 1;
		WeaponBase->SetWeaponStatus(NewStatus);
	}
	APlaySceneGameModeBase* PlaySceneGameMode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!PlaySceneGameMode)return;
	OnLevelUpDelegate.BindUObject(this, &AAC_WeaponSelectManager::MapLevelChange);
	PlaySceneGameMode->ChangeLevel(World);
}

void AAC_WeaponSelectManager::MapLevelChange()
{

}

