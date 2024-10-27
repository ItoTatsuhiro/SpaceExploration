// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlaySceneGameModeBase.generated.h"

class ALevelInterface;

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API APlaySceneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	ALevelInterface* LevelInterfaceRef;

public:
	// レベルインターフェースをセットする
	void SetLevelInterface(ALevelInterface* LevelInterface)
	{
		LevelInterfaceRef = LevelInterface;
	}

	// 
	UFUNCTION(BlueprintCallable)
	void ChangeLevel(TSoftObjectPtr<UWorld> nextlevel);

};
