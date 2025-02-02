// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/EnemyManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "Character/EnemyBase.h"
#include "Manager/PlaySceneGameModeBase.h"
#include "GameData/EnemyStatusData.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	APlaySceneGameModeBase* PlaySceneGameMode = Cast<APlaySceneGameModeBase>( UGameplayStatics::GetGameMode( GetWorld() ) );

	if (!PlaySceneGameMode)
	{
		UE_LOG(LogClass, Error, TEXT("PlaySceneGameMode の取得に失敗しました"));
		return;
	}

	PlaySceneGameMode->SetEnemyManager(this);

}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AEnemyBase* AEnemyManager::CreateEnemy(int EnemyType, int EnemyLevel, EElement EnemyElement)
{
	UE_LOG(LogClass, Log, TEXT("AEnemyManager::CreateEnemy() : begin process"));

	// "EnemyType" が "EnemyClass" のインデックスの範囲外の場合、"NULL" を返す
	if ( EnemyType < 0 || EnemyType >= EnemyClass.Num() )
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::CreateEnemy() : EnemyType=%d に該当する 敵のモデルクラスが存在しませんでした"), EnemyType);
		return NULL;
	}

	if (!EnemyClass[EnemyType])
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::CreateEnemy() : EnemyClass[%d] が nullptr でした"), EnemyType);
		return NULL;
	}

	// 敵をスポーンさせる
	AEnemyBase* Enemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyClass[EnemyType]);

	// 敵のステータスを設定する
	Enemy->SetCharacterStatus( SetEnemyStatus(EnemyLevel - 1) );

	UE_LOG(LogClass, Log, TEXT("AEnemyManager::CreateEnemy() : end process"));

	return Enemy;
}

FStatus AEnemyManager::SetEnemyStatus(int EnemyLevel)
{
	FStatus NewEnemyStatus;

	if (!EnemyStatusDataTable)
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::SetStatus() : EnemyStatusDataTable がセットされていません"), EnemyLevel + 1);
		return NewEnemyStatus;
	}

	TArray<FName> Names = EnemyStatusDataTable->GetRowNames();

	if (EnemyLevel < 0 || EnemyLevel >= Names.Num())
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::SetStatus() : EnemyLevel=%d に該当する EnemyStatusDataTable のデータがありません"), EnemyLevel + 1);
		return NewEnemyStatus;
	}

	FEnemyStatusData* BaseStatus = EnemyStatusDataTable->FindRow<FEnemyStatusData>( Names[EnemyLevel], FString() );

	if (!BaseStatus)
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::SetStatus() : BaseStatus が nullptr でした"));
		return NewEnemyStatus;
	}

	NewEnemyStatus.MaxHp = BaseStatus->MaxHp;
	NewEnemyStatus.HP = BaseStatus->MaxHp;
	NewEnemyStatus.AttackPower = BaseStatus->Attack;
	NewEnemyStatus.DefencePower = BaseStatus->Defence;
	NewEnemyStatus.Speed = BaseStatus->Speed;
	//// **** NewEnemyStatus. = BaseStatus->Exp; ********************

	return NewEnemyStatus;
}
