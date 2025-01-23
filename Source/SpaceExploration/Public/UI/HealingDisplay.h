// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Library/GameLibrary.h"
#include "HealingDisplay.generated.h"

class UButton;
class UProgressBar;
class UImage;
class UTextBlock;

struct FStatus;

/**
 * 
 */
UCLASS()
class SPACEEXPLORATION_API UHealingDisplay : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NextButton;

	// ログテキストのフレーム
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> LogTextFlame;

	// 現在の処理を表示するテキスト
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LogText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BackgroundImage;

	Sequence HealingSequence;

	// "LogTextBlock" の最大幅
	UPROPERTY(EditAnywhere)
	int LogTextMaxWidth;

	// 処理が終了したか判定
	UPROPERTY()
	bool bIsProcessCompleted;

	// シーケンス経過時間
	UPROPERTY()
	float SequenceElapsedTime;

	// 現在のHP
	UPROPERTY()
	float CurrentHp;

	// 最大HPの設定
	UPROPERTY()
	float MaxHp;

	// 回復処理を完了させる時間
	UPROPERTY(EditAnywhere)
	float HealingCompletionTime;

	// 現在の割合
	UPROPERTY()
	float CurrentRate;

	// 現在のHPゲージの割合のずれ
	UPROPERTY()
	float CurrentRateOffset;

	// ゲージの上昇速度
	UPROPERTY()
	float GaugeRiseSpeed;

	// 回復する割合
	UPROPERTY(EditAnywhere)
	float HealingRate;

protected:
	// --------------------------------------------------------------------------
	// 初期化関数
	// --------------------------------------------------------------------------
	bool Initialize() override;

	// --------------------------------------------------------------------------
	// ウィジェットをAddToViewport関数を呼び出したときに実行される関数
	// --------------------------------------------------------------------------
	void NativeConstruct() override;

public:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// --------------------------------------------------------------------------
	// ボタンを押した時の処理を実行
	// --------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void NextButtonClicked();

	// --------------------------------------------------------------------------
	// ウィジェットで表示するステータス（ 現在のHPと最大H P）をセットする。
	// --------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void SetHpStatus(FStatus& TargetStatus);

	// --------------------------------------------------------------------------
	// 回復画面の処理が完了したか判定を返す
	// --------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	bool IsProcessCompleted()
	{
		return bIsProcessCompleted;
	}

	// --------------------------------------------------------------------------
	// テキストブロックの描画サイズを "MaxWidth" を基に調整する。
	// 
	// TextBlock...調整したいテキストブロック
	// MaxWidth...テキストブロックの最大幅
	// --------------------------------------------------------------------------
	void TextSizeAdjust(UTextBlock* TextBlock, int MaxWidth);

private:

	// ===============================================================================
	
	// *****************************************************
	// ---------- 回復処理の各シーケンス --------------
	// *****************************************************

	// --------------------------------------------------------------------------
	// 回復処理をする前の待機シーケンス
	// --------------------------------------------------------------------------
	bool SeqStandbyForHealing(float delta_time);

	// --------------------------------------------------------------------------
	// 回復の処理や演出を行うシーケンス。
	// --------------------------------------------------------------------------
	bool SeqHealing(float delta_time);

	// --------------------------------------------------------------------------
	// 回復処理の終了後のシーケンス
	// --------------------------------------------------------------------------
	bool SeqEndToHealingProcess(float delta_time);

	// ===============================================================================

};
