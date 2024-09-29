// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"


#include "SequenceManager.generated.h"


// シーケンス用の型の定義
DECLARE_DELEGATE_OneParam(FSequenceDelegate, float);


// シーケンス処理を使用するためのクラス
// シーケンス処理を使用したい場合、このクラスを作成して使用する
// シーケンスを入れる場合、そのためのデリゲートを作成してChangeSequenceに入れる必要がある
// 
// 例) 関数『Function1(引数：float delta_time)』を回したい場合、
// 
// // デリゲート作成
// FSequenceDelegate delegate1 
//		= FSequenceDelegate::CreateUObject(SequenceManager, &USequenceManager::Function1);
// // デリゲートセット
// SequenceManager->updateSequence(&delegate1);
// 
// ※※※※※※注意点※※※※※※
// コンストラクタは複数回実行されないため、BeginPlayにてChangeSequence関数を実行してシーケンスをセットすること！！！
// 
// 
UCLASS()
class SPACEEXPLORATION_API USequenceManager : public UObject
{
	GENERATED_BODY()


public:

	// コンストラクタ
	USequenceManager();


	// シーケンスを新しいものに切り替える処理
	// 引数には使用先で作成したデリゲートを参照で入れる
	void ChangeSequence(FSequenceDelegate* newDelegate);
	// シーケンスを更新する処理
	void updateSequence(const float delta_time);


private:

	// 実行するシーケンス
	FSequenceDelegate* sequence_;

};
