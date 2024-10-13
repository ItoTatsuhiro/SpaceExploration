// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SequenceManager.h"


// コンストラクタ
// 引数で初期シーケンスを設定
USequenceManager::USequenceManager() 
{


}



// シーケンスを新しいものに切り替える処理
void USequenceManager::ChangeSequence(FSequenceDelegate newDelegate) {
	
	sequence_ = newDelegate;

	UE_LOG(LogTemp, Log, TEXT("シーケンス切り替え"));

}



// シーケンスを更新する処理
void USequenceManager::updateSequence(const float delta_time) {


	if (!sequence_.IsBound()) {
		return;
	}


	// 実行する
	sequence_.Execute(delta_time);


}
