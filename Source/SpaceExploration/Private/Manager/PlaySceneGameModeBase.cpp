// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/PlaySceneGameModeBase.h"
#include "LevelGroup/LevelInterface.h"
#include "Kismet/KismetSystemLibrary.h"


void APlaySceneGameModeBase::ChangeLevel(TSoftObjectPtr<UWorld> NextLevel)
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

	LevelInterfaceRef->ChangeLevel(NextLevel);
}
