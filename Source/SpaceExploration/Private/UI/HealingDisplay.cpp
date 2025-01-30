// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HealingDisplay.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "tsutsumi/Status.h"



// --------------------------------------------------------------------------
// 初期化関数
// --------------------------------------------------------------------------
bool UHealingDisplay::Initialize()
{
	UE_LOG(LogClass, Log, TEXT("UHealingDisplay::Initialize() : 実行開始"));

	Super::Initialize();

	NextButton->OnClicked.AddUniqueDynamic(this, &UHealingDisplay::NextButtonClicked);

	HealingSequence.BindUObject(this, &UHealingDisplay::SeqStandbyForHealing);

	SequenceElapsedTime = 0.0f;

	CurrentHp = 1.0f;
	MaxHp = 1.0f;

	CurrentRate = 0.0f;

	CurrentRateOffset = CurrentRate;

	return true;
}

// --------------------------------------------------------------------------
// ウィジェットをAddToViewport関数を呼び出したときに実行される関数
// --------------------------------------------------------------------------
void UHealingDisplay::NativeConstruct()
{
	UE_LOG(LogClass, Log, TEXT("UHealingDisplay::NativeConstruct() : 実行開始"));

	Super::NativeConstruct();

	// シーケンスを初期化
	HealingSequence.BindUObject(this, &UHealingDisplay::SeqStandbyForHealing);

	if (HpBar)
	{
		// 現在のHPを"ProgressBar"に設定
		CurrentRate = FMath::Min(CurrentHp / MaxHp, 1.0f);
		CurrentRateOffset = CurrentRate;
		HpBar->SetPercent(CurrentRate);
	}

	// ログテキストの初期化
	if (LogText)
	{
		LogText->SetText(FText::FromString("STUNDBY"));
	}

	bIsProcessCompleted = false;

	if (NextButton)
	{
		NextButton->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UHealingDisplay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HealingSequence.Execute(InDeltaTime);

}

// --------------------------------------------------------------------------
// ボタンを押した時の処理を実行
//  
// * 回復画面の処理完了のフラグを立てる
// --------------------------------------------------------------------------
void UHealingDisplay::NextButtonClicked()
{
	bIsProcessCompleted = true;
}

// --------------------------------------------------------------------------
// ウィジェットで表示するステータス（ 現在のHPと最大H P）をセットする。
// --------------------------------------------------------------------------
void UHealingDisplay::SetHpStatus(FStatus& TargetStatus)
{
	CurrentHp = TargetStatus.HP;
	MaxHp = TargetStatus.MaxHp;
	UE_LOG(LogClass, Log, TEXT( "UHealingDisplay::SetStatus : CurrentHp = %.1f, MaxHp = %.1f"), CurrentHp, MaxHp);
}

// --------------------------------------------------------------------------
// テキストブロックの描画サイズを "MaxWidth" を基に調整する。
// 
// TextBlock...調整したいテキストブロック
// MaxWidth...テキストブロックの最大幅
// --------------------------------------------------------------------------
void UHealingDisplay::TextSizeAdjust(UTextBlock* TextBlock, int MaxWidth)
{
	if (!TextBlock)
	{
		return;
	}

	TextBlock->ForceLayoutPrepass();

	FVector2D DesiredSize = TextBlock->GetDesiredSize();

	if ( !( DesiredSize.X > MaxWidth ) )
	{
		return;
	}

	FVector2D RenderScale = FVector2D( ( MaxWidth / DesiredSize.X ), 1.0f );

	// 描画サイズを調整
	TextBlock->SetRenderScale(RenderScale);

}

// --------------------------------------------------------------------------
// 回復処理をする前の待機シーケンス
// --------------------------------------------------------------------------
bool UHealingDisplay::SeqStandbyForHealing(float delta_time)
{
	SequenceElapsedTime += delta_time;

	// 現在のシーケンスの経過時間が 3 秒未満の場合、処理を抜ける。
	if (SequenceElapsedTime < 3.0f)
	{
		return true;
	}

	// 現在のHPが最大HP以上の場合、SeqEndToHealingProcess関数に切り替え。
	if (CurrentHp >= MaxHp)
	{
		LogText->SetText(FText::FromString("REPAIRS COMPLETED"));
		TextSizeAdjust(LogText, LogTextMaxWidth);

		HealingSequence.BindUObject(this, &UHealingDisplay::SeqEndToHealingProcess);
		SequenceElapsedTime = 0.0f;
		HpBar->SetPercent(FMath::Min(1.0f, CurrentRate + HealingRate));
		NextButton->SetVisibility(ESlateVisibility::Visible);
		return true;
	}

	// シーケンスをSeqHealing関数に切り替え
	HealingSequence.BindUObject(this, &UHealingDisplay::SeqHealing);
	SequenceElapsedTime = 0.0f;

	// 現在の回復値を"ProgresBar"にセット
	CurrentRate = FMath::Min(CurrentHp / MaxHp, 1.0f);
	CurrentRateOffset = CurrentRate;

	LogText->SetText(FText::FromString("NOW REPAIRING..."));
	TextSizeAdjust(LogText, LogTextMaxWidth);

	// ゲージの上昇速度を計算
	GaugeRiseSpeed = HealingRate / HealingCompletionTime;

	return true;
}


// --------------------------------------------------------------------------
// 回復の処理や演出を行うシーケンス。
// --------------------------------------------------------------------------
bool UHealingDisplay::SeqHealing(float delta_time)
{
	SequenceElapsedTime += delta_time;

	CurrentRateOffset += GaugeRiseSpeed * delta_time;

	// 完了時間に達した場合、シーケンスを移動
	if (SequenceElapsedTime >= HealingCompletionTime)
	{
		LogText->SetText(FText::FromString("REPAIRS COMPLETED"));
		TextSizeAdjust(LogText, LogTextMaxWidth);

		HealingSequence.BindUObject(this, &UHealingDisplay::SeqEndToHealingProcess);
		SequenceElapsedTime = 0.0f;
		HpBar->SetPercent(FMath::Min(1.0f, CurrentRate + HealingRate));
		NextButton->SetVisibility(ESlateVisibility::Visible);
		return true;
	}

	HpBar->SetPercent(CurrentRateOffset);

	return true;
}

// --------------------------------------------------------------------------
// 回復処理の終了後のシーケンス
// --------------------------------------------------------------------------
bool UHealingDisplay::SeqEndToHealingProcess(float delta_time)
{
	return true;
}
