// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/PlaySceneGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "LevelGroup/LevelInterface.h"
#include "UI/HealingDisplay.h"
#include "Kismet/KismetSystemLibrary.h"


APlaySceneGameModeBase::APlaySceneGameModeBase() : StageMapData(nullptr), LevelInterfaceRef(nullptr), 
	HealingDisplayClass(nullptr), HealingDisplayWidget(nullptr)
{

}

void APlaySceneGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// 回復画面ウィジェットのセットアップ
	if (HealingDisplayClass && PlayerController)
	{
		HealingDisplayWidget = Cast<UHealingDisplay>( UWidgetBlueprintLibrary::Create( GetWorld(), HealingDisplayClass, PlayerController ) );
	}
}

void APlaySceneGameModeBase::ChangeLevel(TSoftObjectPtr<UWorld> NextLevel, TSoftObjectPtr<UWorld> NowLevel, bool BeforeLevelUnload, bool BeforeLevelVisible)
{
	if (!NextLevel) {
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("APlaySceneGameMode:NextLevelが空です。")), true, true, FColor::Yellow, 2.f, TEXT(""));
		UE_LOG(LogClass, Warning, TEXT("APlaySceneGameMode:NextLevelが空です。"));
	}

	if (!LevelInterfaceRef) 
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("APlaySceneGameMode:ChangeLevel関数を実行できませんでした。")), true, true, FColor::Yellow, 2.f, TEXT(""));
		UE_LOG(LogClass, Warning, TEXT("APlaySceneGameMode:ChangeLevel関数を実行できませんでした。"));
		return;
	}

	LevelInterfaceRef->ChangeLevel(NextLevel, NowLevel, BeforeLevelUnload, BeforeLevelVisible);
}

bool APlaySceneGameModeBase::TryGetStageMapData(FStageMapData* _StageMapData)
{
	if (!StageMapData) 
	{
		UE_LOG(LogClass, Warning, TEXT("APlaySceneGameModeBase::TryGetStageMapData(FStageMapData* _StageMapData) : StageMapDataがnullptrでした。"));
		return false;
	}

	_StageMapData = StageMapData;
	return true;
}

UHealingDisplay* APlaySceneGameModeBase::GetHealingDisplayWhidget()
{
	if (HealingDisplayWidget)
	{
		return HealingDisplayWidget;
	}

	return nullptr;
}

void APlaySceneGameModeBase::SetWepoanManager(AWeaponManager* WeaponManager)
{
	WeaponManagerRef = WeaponManager;
}

void APlaySceneGameModeBase::SetEnemyManager(AEnemyManager* EnemyManager)
{
	EnemyManagerRef = EnemyManager;
}

void APlaySceneGameModeBase::SetStageDataManager(AStageDataManager* StageDataManager)
{
	StageDataManagerRef = StageDataManager;
}
