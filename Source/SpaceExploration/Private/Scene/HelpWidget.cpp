// Fill out your copyright notice in the Description page of Project Settings.

#include "Scene/HelpWidget.h"
#include "Components/Button.h"
#include "Scene/BP_MapSceneUIManager.h"
#include "Kismet/GameplayStatics.h"

void UHelpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Button_Close‚ð•R‚Ã‚¯‚é
	Button_Close = Cast<UButton>(GetWidgetFromName("Button_Close"));
	if (Button_Close) {
		Button_Close->OnClicked.AddUniqueDynamic(this, &UHelpWidget::OnButton_Close);
	}
}

bool UHelpWidget::Initialize()
{
	Super::Initialize();
	return false;
}

void UHelpWidget::OnButton_Close()
{
	Cast<ABP_MapSceneUIManager>(mapsceneuimanager)->visibleHelpUIWidget(false);
	Cast<ABP_MapSceneUIManager>(mapsceneuimanager)->visibleMapSceneUIWidget(true);
}
