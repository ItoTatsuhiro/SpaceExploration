// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleScene/BatllSceneWidget.h"
#include "Components/Button.h"
#include "BattleScene/BattleManager.h"
#include "Kismet/GameplayStatics.h"

void UBatllSceneWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//ButtonBattleEnd‚ÌOnClicked‚ÉuOnButtonBattleEndv‚ğŠÖ˜A‚Ã‚¯‚é
	ButtonBattleEnd = Cast<UButton>(GetWidgetFromName("ButtonBattleEnd"));
	if (ButtonBattleEnd) {
		UE_LOG(LogClass, Warning, TEXT("~yes ButtonBattleEnd~\n"));
		ButtonBattleEnd->OnClicked.AddUniqueDynamic(this, &UBatllSceneWidget::OnButtonBattleEnd);
	}
}

bool UBatllSceneWidget::Initialize()
{
	Super::Initialize();

	return false;
}

void UBatllSceneWidget::OnButtonBattleEnd()
{
	battlemanager->BattleEnd();
	UKismetSystemLibrary::PrintString(this, "~Click ButtonBattleEnd~", true, true, FColor::Cyan, 2.f, TEXT("None"));
	UE_LOG(LogClass, Warning, TEXT("~Click ButtonBattleEnd~\n"));
}
