// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BatllSceneWidget.generated.h"

class UButton;

UCLASS()
class SPACEEXPLORATION_API UBatllSceneWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct()override;

private:
	UPROPERTY()
	TObjectPtr<UButton> BattleStart;

};
