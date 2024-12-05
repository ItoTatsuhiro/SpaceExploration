// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/PlaySceneGameModeBase.h"
#include "LevelGroup/LevelInterface.h"
#include "Kismet/KismetSystemLibrary.h"


APlaySceneGameModeBase::APlaySceneGameModeBase() : StageMapData(nullptr)
{

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
