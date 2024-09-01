// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelInterface.generated.h"

UCLASS()
class SPACEEXPLORATION_API ALevelInterface : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelInterface();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//シーン遷移、次のレベルを読み込み
	UFUNCTION(BlueprintCallable)
	void LoadLevel(TSoftObjectPtr<UWorld> nextlevel);
	//レベル破棄
	UFUNCTION(BlueprintCallable)
	void UnLoadLevel(TSoftObjectPtr<UWorld> deletelevel);

private:
	FLatentActionInfo LatentInfo;
	
	//初期ワールド
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> FirstLevel;

	//現在のワールド
	UPROPERTY(VisibleAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> NowLevel;

	UPROPERTY(VisibleAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> BeforeLevel;
};
