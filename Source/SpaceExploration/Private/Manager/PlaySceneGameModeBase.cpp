// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/PlaySceneGameModeBase.h"
#include "LevelGroup/LevelInterface.h"


void APlaySceneGameModeBase::ChangeLevel(TSoftObjectPtr<UWorld> nextlevel)
{
	LevelInterfaceRef->ChangeLevel(nextlevel);
}
