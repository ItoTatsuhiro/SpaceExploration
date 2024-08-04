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

	//�V�[���J�ځA���̃��x����ǂݍ���
	UFUNCTION(BlueprintCallable)
	void LoadLevel(TSoftObjectPtr<UWorld> nextlevel);
	//�V�[���J�ځA�O�̃��x���j���ALoadLevel�̌�ɌĂ�
	UFUNCTION(BlueprintCallable)
	void UnLoadLevel();

	UFUNCTION(BlueprintCallable)
	void testfanc(int aaa);

private:
	FLatentActionInfo LatentInfo;
	
	//�������[���h
	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> FirstLevel;

	//���݂̃��[���h
	UPROPERTY(VisibleAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> NowLevel;

	UPROPERTY(VisibleAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> BeforeLevel;
};
