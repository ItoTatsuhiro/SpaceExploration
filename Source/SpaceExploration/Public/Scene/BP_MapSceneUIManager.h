// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Scene/SelectTileScene/AC_StageMapManager.h"
#include "../Manager/StageDataManager.h"
#include "../GameData/StageMapData.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "BP_MapSceneUIManager.generated.h"

class AStageDataManager;
class UUserWidget;
class APlaySceneGameModeBase;

UCLASS()
class SPACEEXPLORATION_API ABP_MapSceneUIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABP_MapSceneUIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type _endplay) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//マップシーンUI
	UPROPERTY()
	TSubclassOf<UUserWidget> MapSceneUIWidgetClass;
	UPROPERTY()
	UUserWidget* mapsceneuiwidget = nullptr;
	const FString MapSceneWidgetPath = TEXT("/Game/mapScene/WBP_MapScene.WBP_MapScene_C");

	//ヘルプUI
	UPROPERTY()
	TSubclassOf<UUserWidget> HelpWidgetClass;
	UPROPERTY()
	UUserWidget* helpwidget = nullptr;
	const FString HelpWidgetPath = TEXT("/Game/LevelGroup/WBP_Help.WBP_Help_C");

	//ゲームクリアUI
	UPROPERTY()
	TSubclassOf<UUserWidget> GameClearWidgetClass;
	UPROPERTY()
	UUserWidget* gameclearwidget = nullptr;

	FStageMapData _nowstagedata;

	//ゲームモード
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlaySceneGameModeBase> gamemode;

public:
	//

	//マップシーンUIの表示切り替え
	void visibleMapSceneUIWidget(bool visible) {
		if (mapsceneuiwidget) {
			if (visible) {
				mapsceneuiwidget->SetVisibility(ESlateVisibility::Visible);
			}
			else {
				mapsceneuiwidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	//ヘルプUIの表示切替
	void visibleHelpUIWidget(bool visible) {
		if (helpwidget) {
			if (visible) {
				helpwidget->SetVisibility(ESlateVisibility::Visible);
			}
			else {
				helpwidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
};
