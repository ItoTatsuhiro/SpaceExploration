// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponInventoryComponent.generated.h"


class AWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEXPLORATION_API UWeaponInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 各属性の武器データの配列を返す
	UFUNCTION(BlueprintCallable)
	TArray< AWeaponBase* >& GetElementWeapons() { return ElementWeapons; }

	// 設定したクラスから武器の生成を行う
	UFUNCTION()
	void CreateElememtWeapons();

protected:

	// 各属性の武器アクターデータ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TArray< AWeaponBase* > ElementWeapons;

	// 生成する武器のクラスデータ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TArray< TSubclassOf<AWeaponBase> > ElementWeaponsClass;

};
