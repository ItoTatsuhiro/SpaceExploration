// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/EnemyManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "Character/EnemyBase.h"
#include "Manager/PlaySceneGameModeBase.h"
#include "GameData/StatusData.h"
#include "Manager/WeaponManager.h"
#include "Weapon/WeaponBase.h"

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
	
	EnemyStatusDataTable.LoadSynchronous();

	APlaySceneGameModeBase* PlaySceneGameMode = Cast<APlaySceneGameModeBase>( UGameplayStatics::GetGameMode( GetWorld() ) );

	if (!PlaySceneGameMode)
	{
		UE_LOG(LogClass, Error, TEXT("PlaySceneGameMode の取得に失敗しました"));
		return;
	}

	PlaySceneGameMode->SetEnemyManager(this);

	// WeaponManagerRef の取得をタイマーで定期的に試みる
	GetWorldTimerManager().SetTimer(EnemyManagerProcessHandle, this, &AEnemyManager::CheckWeaponManager, 0.1f, true);

}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// -----------------------------------------------------------------
// 敵の生成を行う
// 
// 引数
// EnemyType...		敵の種類
// EnemyLevel...	敵のレベル
// EnemyElement...	敵の属性
// -----------------------------------------------------------------
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

	if (!WeaponManagerRef) 
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::CreateEnemy() : WeaponManagerRef が nullptr でした"));
		return Enemy;
	}

	// 敵の武器生成
	AWeaponBase* Weapon = WeaponManagerRef->CreateWeapon(1, EnemyElement);

	if (!Weapon)
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::CreateEnemy() : Weapon の生成に失敗しました"));
		return Enemy;
	}

	// 生成した武器を装備
	Weapon->SetWeaponMeshHiddenInGame(true);
	Enemy->SetEquippedWeapon(Weapon);

	UE_LOG(LogClass, Log, TEXT("AEnemyManager::CreateEnemy() : end process"));

	return Enemy;
}

// -----------------------------------------------------------------
// WeaponManagerの参照が存在するか確認し、セットする
// -----------------------------------------------------------------
void AEnemyManager::CheckWeaponManager()
{
	APlaySceneGameModeBase* PlaySceneGameMode = Cast<APlaySceneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (PlaySceneGameMode)
	{
		WeaponManagerRef = PlaySceneGameMode->GetWeaponManager();

		if (WeaponManagerRef)
		{
			UE_LOG(LogClass, Log, TEXT("WeaponManagerRef を取得しました"));
			// WeaponManagerRef を取得したのでタイマーを解除
			GetWorldTimerManager().ClearTimer(EnemyManagerProcessHandle);
		}
	}
}

// -----------------------------------------------------------------
// 敵のステータスを設定する
// 
// 引数
// EnemyLevel...敵のレベル
// -----------------------------------------------------------------
FStatus AEnemyManager::SetEnemyStatus(int EnemyLevel)
{
	// 新しくセットするステータス
	FStatus NewEnemyStatus;

	// データテーブルをロード
	EnemyStatusDataTable.LoadSynchronous();

	if (!EnemyStatusDataTable)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("EnemyStatusDataTable がセットされていません")), true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::SetStatus() : EnemyStatusDataTable がセットされていません"));
		return NewEnemyStatus;
	}

	// EnemyStatusDataTable のデータを取得
	TArray<FName> Names = EnemyStatusDataTable->GetRowNames();

	if (EnemyLevel < 0 || EnemyLevel >= Names.Num())
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::SetStatus() : EnemyLevel=%d に該当する EnemyStatusDataTable のデータがありません"), EnemyLevel + 1);
		return NewEnemyStatus;
	}

	// EnemyLevelに一致する列のステータスを取得
	FStatusData* BaseStatus = EnemyStatusDataTable->FindRow<FStatusData>( Names[EnemyLevel], FString() );

	if (!BaseStatus)
	{
		UE_LOG(LogClass, Error, TEXT("AEnemyManager::SetStatus() : BaseStatus が nullptr でした"));
		return NewEnemyStatus;
	}

	// 各ステータスの設定
	NewEnemyStatus.MaxHp = BaseStatus->BaseMaxHp + (FMath::Rand() % BaseStatus->RandomMaxHp);
	NewEnemyStatus.HP = NewEnemyStatus.MaxHp;
	NewEnemyStatus.AttackPower = BaseStatus->BaseAttack + (FMath::Rand() % BaseStatus->RandomAttack);
	NewEnemyStatus.DefencePower = BaseStatus->BaseDefence + (FMath::Rand() % BaseStatus->RandomDefence);
	NewEnemyStatus.Speed = BaseStatus->BaseSpeed + (FMath::Rand() % BaseStatus->RandomSpeed);
	NewEnemyStatus.Exp = BaseStatus->Exp;

	return NewEnemyStatus;
}
