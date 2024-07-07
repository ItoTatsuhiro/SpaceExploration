// Fill out your copyright notice in the Description page of Project Settings.


#include "tsutsumi/GalaxyRandomSelect.h"
#include "Scene\SelectTileScene\E_Tile.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Scene/SelectTileScene/E_Tile.h"





// Sets default values
AGalaxyRandomSelect::AGalaxyRandomSelect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}


//—”
int RandomProgram() {
	return std::rand() % 3;
}
// Called when the game starts or when spawned
void AGalaxyRandomSelect::BeginPlay()
{
  
	Super::BeginPlay();

	//forƒ‹[ƒv‚Åˆê‚Â‚ÌƒXƒe[ƒW‚Ì˜f¯‚ğ“ñŸŒ³”z—ñ‚ÉŠi”[‚µ‚Ä‚¢‚Ü‚·B[0][0]‚Æ[2][0]‚É‚Í‰½‚àŠi”[‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			if (i == 0 && j == 0) {}
			else if (i == 2 && j == 0) {}
			else {
				//—”‚©‚ç˜f¯‚ğPlanetLine”z—ñ‚ÉŠi”[‚µ‚Ä‚¢‚Ü‚·
				int random = RandomProgram();
				if (random == static_cast<int>(E_TILE_TYPE::BATTLE)) {
					PlanetLine[i][j] = E_TILE_TYPE::BATTLE;
				}
				else if (random == static_cast<int>(E_TILE_TYPE::BATTLE)) {
				    PlanetLine[i][j] = E_TILE_TYPE::ITEM;
				}
				else if (random == static_cast<int>(E_TILE_TYPE::HEAL))  { 
					PlanetLine[i][j] = E_TILE_TYPE::HEAL;
				}
			
			}
		 }
	}
}

// Called every frame
void AGalaxyRandomSelect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

