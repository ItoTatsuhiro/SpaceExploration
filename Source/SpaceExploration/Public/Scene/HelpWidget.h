// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HelpWidget.generated.h"

class UButton;

UCLASS()
class SPACEEXPLORATION_API UHelpWidget : public UUserWidget
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
	TObjectPtr<UButton> Button_Close;
	
public:
	//Button_Closeを押した時の処理
	UFUNCTION(BlueprintCallable)
	void OnButton_Close();

	//セッター ABP_MapSceneUIManager
	void SetMapSceneUIManaget(AActor* _mapsceneuimanager) {
		mapsceneuimanager = _mapsceneuimanager;
	}
};
