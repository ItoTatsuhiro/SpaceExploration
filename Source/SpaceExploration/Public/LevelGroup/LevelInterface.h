// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelInterface.generated.h"

class ACharacterBase;

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

	//シーン変更（シーン変更時に一度だけ呼んでね）
	//引数１：次のレベル
	void ChangeLevel(TSoftObjectPtr<UWorld> nextlevel);

	//ローディング画面レベル読み込み
	UFUNCTION()
	void Load_LoadingLevel();
	//次のレベル読み込み
	UFUNCTION()
	void Load_NextLevel();
	//ローディング画面レベル消去
	UFUNCTION()
	void Unload_LoadingLevel();

	//テスト用時間 delay関係
	UFUNCTION()
	void testdelay();
	UPROPERTY()
	UWorld* world;
	
//------------------------------------------------------
//セッター


//ゲッター

private:

	//ローディング画面
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> LoadingLevel = nullptr;

	//次のレベル（エディタは初期レベル設定）
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> NextLevel = nullptr;

	//現在のレベル
	UPROPERTY(VisibleAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> NowLevel;

	//一つ前のレベル
	UPROPERTY(VisibleAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> BeforeLevel;
};
