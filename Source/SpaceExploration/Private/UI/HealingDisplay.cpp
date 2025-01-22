// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HealingDisplay.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UHealingDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	NextButton->OnClicked.AddUniqueDynamic(this, &UHealingDisplay::NextButtonClicked);

	HealingSequence.BindUObject(this, &UHealingDisplay::SeqStandbyForHealing);

	SequenceElapsedTime = 0.0f;

	HealingCompletionTime = 3.0f;

	CurrentRate = 0.0f;

	CurrentRateOffset = CurrentRate;

	HealingRate = 0.4;

}

bool UHealingDisplay::Initialize()
{
	Super::Initialize();

	// シーケンスを初期化
	HealingSequence.BindUObject(this, &UHealingDisplay::SeqStandbyForHealing);

	if ( HpBar )
	{
		// 現在のHPを"ProgressBar"に設定
		CurrentRate = FMath::Min(Hp / MaxHp, 1.0f);
		CurrentRateOffset = CurrentRate;
		HpBar->SetPercent(CurrentRate);
	}

	if ( LogText )
	{
		LogText->SetText(FText::FromString("STUNDBY"));
	}
	
	bIsProcessCompleted = false;

	if ( NextButton )
	{
		NextButton->SetVisibility(ESlateVisibility::Hidden);
	}

	return true;
}

void UHealingDisplay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HealingSequence.Execute(InDeltaTime);

}

void UHealingDisplay::NextButtonClicked()
{
	bIsProcessCompleted = true;
}

bool UHealingDisplay::SeqStandbyForHealing(float delta_time)
{
	SequenceElapsedTime += delta_time;

	if (SequenceElapsedTime >= 3.0f)
	{
		// シーケンスを切り替え
		HealingSequence.BindUObject(this, &UHealingDisplay::SeqHealing);
		SequenceElapsedTime = 0.0f;

		// 現在の回復値を"ProgresBar"にセット
		CurrentRate = FMath::Min(Hp / MaxHp, 1.0f);
		CurrentRateOffset = CurrentRate;

		LogText->SetText(FText::FromString("NOW REPAIRING..."));

		// ゲージの上昇速度を計算
		GaugeRiseSpeed = HealingRate / HealingCompletionTime;
	}

	return true;
}

bool UHealingDisplay::SeqHealing(float delta_time)
{
	SequenceElapsedTime += delta_time;

	CurrentRateOffset += GaugeRiseSpeed * delta_time;

	// 完了時間に達した場合、シーケンスを移動
	if (SequenceElapsedTime >= HealingCompletionTime)
	{
		LogText->SetText(FText::FromString("REPAIRS COMPLETED"));

		HealingSequence.BindUObject(this, &UHealingDisplay::SeqEndToHealingProcess);
		SequenceElapsedTime = 0.0f;
		HpBar->SetPercent(FMath::Min(1.0f, CurrentRate + HealingRate));
		NextButton->SetVisibility(ESlateVisibility::Visible);
		return true;
	}

	HpBar->SetPercent(CurrentRateOffset);

	return true;
}

bool UHealingDisplay::SeqEndToHealingProcess(float delta_time)
{
	return true;
}
