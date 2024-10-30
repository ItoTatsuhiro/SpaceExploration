// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Manager/SequenceManager.h"


#include "AC_SceneManagerBase.generated.h"



// シーンマネージャーベースクラス
// シーン上の動作を管理するためのマネージャークラスのベースとして使用する用
UCLASS()
class SPACEEXPLORATION_API AAC_SceneManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_SceneManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
protected:


	// プレイヤーの参照
	UPROPERTY(VisibleAnywhere)
	class APlayerCharacter* playerCharacter_;


	// シーケンスマネージャー
	// 継承先でこれを利用して処理を行う
	// 使用例はAC_StageMapManagerを確認すること
	UPROPERTY(VisibleAnywhere)
	USequenceManager* sequenceManager_;



	// レイを飛ばして当たったActorを取得する関数
	UFUNCTION(BlueprintCallable)
	AActor* PerformRaycast();


//----------------------------------------------------
// 入力を受け取る処理の関係

public:

	// クリック入力を外部から伝えるための関数
	// playerControllerでクリックされたときの処理としてバインドする用
	void OnClickInput();


protected:

	// クリック入力があった際に一瞬trueにするための関数
	// updateで確認してfalseにする
	UPROPERTY()
	bool isClickInput_ = false;



	// クリック入力を確認する関数
	// クリック入力を必要とする処理と、update関数で毎回isClickInputを確認する
	bool CheckClickInput();


};
