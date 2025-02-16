// Fill out your copyright notice in the Description page of Project Settings
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
	//クリックをしているか判定
	OnLevelUpDelegate.Execute();
	CheckClickInput();
	Super::Tick(DeltaTime);
}
void AAC_WeaponSelectManager::WeaponLevelUp()
{
	// クリックが行われていない場合、抜ける
	if (!isClickInput_) 
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) 
	{
		return;
	}
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(PerformRaycast());
	if (!WeaponBase)
	{
		UE_LOG(LogClass, Log, TEXT("AAC_WeaponManager::WeaponLevelUp() : error WeaponBase is nullptr"));
		return;
	}
	if (!playerCharacter_)return;
	APlaySceneGameModeBase* PlaySceneGameMode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(this));
	playerCharacter_->SetEquippedWeapon(WeaponBase);
	if (!PlaySceneGameMode)return;
	UE_LOG(LogClass, Log, TEXT("AAC_WeaponManager::WeaponLevelUp() : 武器を取得を完了"));
	OnLevelUpDelegate.BindUObject(this, &AAC_WeaponSelectManager::MapLevelChange);
	PlaySceneGameMode->ChangeLevel(nextLevel,GetWorld());
	WindowClose();
}

void AAC_WeaponSelectManager::MapLevelChange()
{
}
void AAC_WeaponSelectManager::WindowClose()
{
}
