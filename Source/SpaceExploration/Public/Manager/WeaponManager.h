// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "tsutsumi/Status.h"
#include "tsutsumi/Element.h"
#include "WeaponManager.generated.h"

class AWeaponBase;

/*
* 作成者：前
* 武器の生成やステータスの設定を行うアクタークラス
*/
UCLASS()
class SPACEEXPLORATION_API AWeaponManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	AWeaponBase* CreateWeapon(int WeaponLevel, EElement WeaponElement);

private:
	FStatus SetStatus(int WeaponLevel);

private:
	// 生成する武器クラス配列
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray< TSubclassOf< AWeaponBase > > CreateWeaponClass;

	// 武器の各レベルごとのステータスのデータテーブル
	UPROPERTY(EditAnywhere, Category = "DataTable")
	TSoftObjectPtr<UDataTable> WeaponStatusDataTable;

};
