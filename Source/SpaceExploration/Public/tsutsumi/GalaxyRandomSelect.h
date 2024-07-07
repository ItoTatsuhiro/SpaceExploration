// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Scene/SelectTileScene/AC_MapTile.h"
#include "../Scene/SelectTileScene/E_Tile.h"
#include "GalaxyRandomSelect.generated.h"

UCLASS()
class SPACEEXPLORATION_API AGalaxyRandomSelect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGalaxyRandomSelect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//�f���̔z��
	E_TILE_TYPE PlanetLine[3][3];

private:
	// �K�v�ɉ����āA�v���C�x�[�g�����o�[��ǉ����Ă�������
};
