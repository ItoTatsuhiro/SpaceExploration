// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "LevelGroup/LevelInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ALevelInterface::ALevelInterface()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelInterface::BeginPlay()
{
	Super::BeginPlay();
	
	
	//�������[���h�ٓǂݍ���
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, FirstLevel, true, true, LatentInfo);

	//NowLevel��ݒ�
	NowLevel = FirstLevel;
	
}

// Called every frame
void ALevelInterface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//BeforeLevel�ɒ��g�������Unload����������
	if (BeforeLevel != nullptr) {
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, BeforeLevel, LatentInfo, true);

		//BeforeLevel�����i�J���j
		BeforeLevel.Reset();
		BeforeLevel = nullptr;
	}
}

void ALevelInterface::LoadLevel(TSoftObjectPtr<UWorld> nextlevel)
{
	//���݂̃��x����O�̃��x���ɂ���
	BeforeLevel = NowLevel;
	//�V�������݂̃��x���ݒ�
	NowLevel = nextlevel;

	//���̃��x����ǂݍ���
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NowLevel, true, true, LatentInfo);
}

void ALevelInterface::UnLoadLevel()
{
	//���݂̃��x��������
	
}

void ALevelInterface::testfanc(int aaa)
{
	UKismetSystemLibrary::PrintString(this, "!!!test fanc printstring!!!", true, true, FColor::Cyan, 2.f, TEXT("None"));
}
