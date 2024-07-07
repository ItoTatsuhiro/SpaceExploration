// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../tsutsumi/CPPCalcInfo.h"
#include "../tsutsumi/Element.h"
#include "WeaponBase.generated.h"

UCLASS(Abstract)
class SPACEEXPLORATION_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 武器の位置を取得する
	inline const FVector3f& GetWeaponLocation() const { return WeaponLocation; }
	// 武器のステータスを取得する
	inline const FStatus& GetWeaponStatus() const { return WeaponStatus; }
	//// 武器の属性を取得する
	//inline const EElement GetWeaponElement() const { return WeaponElement; }

	// ==========================================================================
	// ここから
	// ==========================================================================
	// 武器の位置を設定する
	inline void SetWeaponLocation(const FVector3f& Location) { WeaponLocation = Location; }
	// inline void SetWeaponStatus(const FStatus& Status) { WeaponStatus = Status; }

	// 武器のステータスを設定する
	void SetWeaponStatus(const FStatus& Status);

protected:
	// 武器の位置
	UPROPERTY(EditAnywhere)
	FVector3f WeaponLocation;

	// 武器のステータス
	UPROPERTY(EditAnywhere)
	FStatus WeaponStatus;

	//// 武器の属性
	//UPROPERTY(EditAnywhere)
	//EElement WeaponElement;

};
