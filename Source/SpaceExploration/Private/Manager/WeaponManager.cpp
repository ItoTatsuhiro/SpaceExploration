// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/WeaponManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "Character/EnemyBase.h"
#include "Manager/PlaySceneGameModeBase.h"
#include "GameData/StatusData.h"

// Sets default values
AWeaponManager::AWeaponManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	WeaponStatusDataTable.LoadSynchronous();

	APlaySceneGameModeBase* PlaySceneGameMode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!PlaySceneGameMode)
	{
		UE_LOG(LogClass, Error, TEXT("PlaySceneGameMode の取得に失敗しました"));
		return;
	}

	 PlaySceneGameMode->SetWepoanManager(this);

}

// Called every frame
void AWeaponManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AWeaponBase* AWeaponManager::CreateWeapon(int WeaponLevel, EElement WeaponElement)
{
	UE_LOG(LogClass, Log, TEXT("AWeaponManager::CreateWeapon() : begin process"));

	int WeaponElementValue = static_cast<uint8>(WeaponElement);

	// "EnemyType" が "EnemyClass" のインデックスの範囲外の場合、"NULL" を返す
	if (WeaponElementValue < 0 || WeaponElementValue >= CreateWeaponClass.Num())
	{
		UE_LOG(LogClass, Error, TEXT("AWeaponManager::CreateWeapon() : EElement=%d に該当する 属性が存在しませんでした"), WeaponElementValue);
		return NULL;
	}

	if (!CreateWeaponClass[WeaponElementValue])
	{
		UE_LOG(LogClass, Error, TEXT("AWeaponManager::CreateWeapon() : EnemyClass[%d] が nullptr でした"), WeaponElementValue);
		return NULL;
	}

	// 敵をスポーンさせる
	AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(CreateWeaponClass[WeaponElementValue]);

	// 敵のステータスを設定する
	Weapon->SetWeaponStatus(SetStatus(WeaponLevel - 1));

	UE_LOG(LogClass, Log, TEXT("AWeaponManager::CreateWeapon() : end process"));

	return Weapon;
}

FStatus AWeaponManager::SetStatus(int WeaponLevel)
{
	FStatus NewWeaponStatus;

	WeaponStatusDataTable.LoadSynchronous();

	if (!WeaponStatusDataTable)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("AWeaponManager::SetStatus() : WeaponStatusDataTable がセットされていません。")), true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogClass, Error, TEXT("AWeaponManager::SetStatus() : WeaponStatusDataTable がセットされていません。"));
		return NewWeaponStatus;
	}

	TArray<FName> Names = WeaponStatusDataTable->GetRowNames();

	if (WeaponLevel < 0 || WeaponLevel >= Names.Num())
	{
		UE_LOG( LogClass, Error, TEXT( "AWeaponManager::SetStatus() : EnemyLevel=%d に該当する EnemyStatusDataTable のデータがありません" ), WeaponLevel + 1 );
		return NewWeaponStatus;
	}

	FStatusData* BaseStatus = WeaponStatusDataTable->FindRow<FStatusData>(Names[WeaponLevel], FString());

	if (!BaseStatus)
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::SetStatus() : BaseStatus が nullptr でした"));
		return NewWeaponStatus;
	}

	NewWeaponStatus.MaxHp = BaseStatus->BaseMaxHp + (FMath::Rand() % BaseStatus->RandomMaxHp);
	NewWeaponStatus.HP = NewWeaponStatus.MaxHp;
	NewWeaponStatus.AttackPower = BaseStatus->BaseAttack + (FMath::Rand() % BaseStatus->RandomAttack);
	NewWeaponStatus.DefencePower = BaseStatus->BaseDefence + (FMath::Rand() % BaseStatus->RandomDefence);
	NewWeaponStatus.Speed = BaseStatus->BaseSpeed + (FMath::Rand() % BaseStatus->RandomSpeed);

	return NewWeaponStatus;
}
