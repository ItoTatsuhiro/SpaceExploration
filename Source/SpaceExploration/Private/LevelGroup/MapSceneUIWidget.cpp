// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGroup/MapSceneUIWidget.h"
#include "Components/Button.h"
#include "Scene/BP_MapSceneUIManager.h" 
#include "Kismet/GameplayStatics.h"

void UMapSceneUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Button_Help‚ð•R‚Ã‚¯‚é
	//Button_Help = Cast<UButton>(GetWidgetFromName("Button_Help"));
	//if (Button_Help) {
	Button_Help->OnClicked.AddUniqueDynamic(this, &UMapSceneUIWidget::OnButton_Help);
		UE_LOG(LogClass, Log, TEXT("UMapSceneUIWidget : AddUniqueDynamic"));
	//}

	UE_LOG(LogClass, Log, TEXT("UMapSceneUIWidget : NativeConstruct"));

}

bool UMapSceneUIWidget::Initialize()
{
	Super::Initialize();
	return false;
}

void UMapSceneUIWidget::OnButton_Help()
{
	UE_LOG(LogClass, Log, TEXT("UMapSceneUIWidget : Button_help click"));

	Cast<ABP_MapSceneUIManager>(mapsceneuimanager)->visibleHelpUIWidget(true);
	Cast<ABP_MapSceneUIManager>(mapsceneuimanager)->visibleMapSceneUIWidget(false);
}
