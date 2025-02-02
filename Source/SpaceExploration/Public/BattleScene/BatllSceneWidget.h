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
	bool Initialize()override;

public:
	UPROPERTY(BlueprintReadOnly)
	AActor* battlemanager;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonBattleEnd;

public:
	UFUNCTION(BlueprintCallable)
	void OnButtonBattleEnd();

public:
	//セッター
	void SetBattleManager(AActor* _battleManager)
	{
		battlemanager = _battleManager;
	}

};
