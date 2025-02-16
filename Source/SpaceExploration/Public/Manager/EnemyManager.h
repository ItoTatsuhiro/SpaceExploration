// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "tsutsumi/Status.h"
#include "tsutsumi/Element.h"
#include "EnemyManager.generated.h"

class AEnemyBase;
class AWeaponManager;


/*
* 作成者：前
* 敵の生成やステータスの管理を行うクラス
*/
UCLASS()
class SPACEEXPLORATION_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// -----------------------------------------------------------------
	// 敵の生成を行う
	// 
	// 引数
	// EnemyType...		敵の種類
	// EnemyLevel...	敵のレベル
	// EnemyElement...	敵の属性
	// -----------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	AEnemyBase* CreateEnemy(int EnemyType, int EnemyLevel, EElement EnemyElement);

private:
	// -----------------------------------------------------------------
	// WeaponManagerの参照が存在するか確認し、セットする
	// -----------------------------------------------------------------
	void CheckWeaponManager();

	// -----------------------------------------------------------------
	// 敵のステータスを設定する
	// 
	// 引数
	// EnemyLevel...敵のレベル
	// -----------------------------------------------------------------
	UFUNCTION()
	FStatus SetEnemyStatus(int EnemyLevel);

private:
	// 生成を行うエネミーのデータ
	UPROPERTY(EditDefaultsOnly)
	TArray< TSubclassOf< AEnemyBase > > EnemyClass;

	// 敵のステータスのデータテーブル
	UPROPERTY(EditAnywhere , Category = "DataTable")
	TSoftObjectPtr<UDataTable> EnemyStatusDataTable;

	// 敵の武器を生成する用のクラス参照
	UPROPERTY(VisibleAnywhere, Category = "WeaponManager")
	AWeaponManager* WeaponManagerRef;

	UPROPERTY()
	FTimerHandle EnemyManagerProcessHandle;

};
