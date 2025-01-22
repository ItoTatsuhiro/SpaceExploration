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

	// 処理が終了したか判定
	UPROPERTY()
	bool bIsProcessCompleted;

	// シーケンス経過時間
	UPROPERTY()
	float SequenceElapsedTime;

	// 現在のHP
	UPROPERTY(EditAnywhere)
	float Hp = 60;

	// 最大HPの設定
	UPROPERTY(EditAnywhere)
	float MaxHp = 120;

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
	void NativeConstruct() override;

	bool Initialize() override;

public:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void NextButtonClicked();

	UFUNCTION(BlueprintCallable)
	bool IsProcessCompleted()
	{
		return bIsProcessCompleted;
	}

private:
	// 回復処理の待機シーケンス
	bool SeqStandbyForHealing(float delta_time);

	// 回復シーケンス
	bool SeqHealing(float delta_time);

	// 回復処理終了のシーケンス
	bool SeqEndToHealingProcess(float delta_time);

};
