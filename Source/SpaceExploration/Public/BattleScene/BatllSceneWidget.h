// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BatllSceneWidget.generated.h"

class UButton;
class ABattleManager;

UCLASS()
class SPACEEXPLORATION_API UBatllSceneWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct()override;
	bool Initialize()override;

private:
	UPROPERTY()
	ABattleManager* battlemanager;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonBattleEnd;

	UFUNCTION(BlueprintCallable)
	void OnButtonBattleEnd();

public:

	void SetBattleManager(ABattleManager* _battleManager)
	{
		battlemanager = _battleManager;
	}

};
