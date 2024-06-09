// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SelectTileScene/AC_MapTile.h"

// Sets default values
AAC_MapTileBase::AAC_MapTileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAC_MapTileBase::BeginPlay()
{
	Super::BeginPlay();
	
}


//void AAC_MapTile::


// Called every frame
void AAC_MapTileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	TileEvent();

}

