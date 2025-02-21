// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSceneUIWidget.generated.h"

class UButton;

UCLASS()
class SPACEEXPLORATION_API UMapSceneUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct()override;
	bool Initialize()override;

public:
	UPROPERTY(BlueprintReadOnly)
	AActor* mapsceneuimanager;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Help;

public:
	//Button_Helpを押した時の処理
	UFUNCTION(BlueprintCallable)
	void OnButton_Help();

	//セッター ABP_MapSceneUIManager
	void SetMapSceneUIManager(AActor* _mapsceneuimanager) {
		mapsceneuimanager = _mapsceneuimanager;
	}
	
};
