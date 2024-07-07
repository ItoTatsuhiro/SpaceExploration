// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanetVariation.generated.h"
UENUM(BlueprintType)
enum class EPlanet : uint8
{
    Fight = 0,
    Heal,
    TreasureBox,
    Max
};
void func() {
    
    FString mess_[static_cast<uint8>(EPlanet::Max)];

    mess_[static_cast<uint8>(EPlanet::Fight)] = "Fight";
    mess_[static_cast<uint8>(EPlanet::Heal)] = "Heal";
    mess_[static_cast<uint8>(EPlanet::TreasureBox)] = "TreasureBox";

  
}
