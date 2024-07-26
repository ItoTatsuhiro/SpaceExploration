// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../tsutsumi/Status.h"
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
	
	// 武器のステータスを取得する
	inline const FStatus& GetWeaponStatus() const { return WeaponStatus; }
	// 武器の属性を取得する
	inline const EElement GetWeaponElement() const { return WeaponElement; }

	// ==========================================================================
	// ここから
	// ==========================================================================
	// 武器のステータスを設定する
	inline void SetWeaponStatus(const FStatus& Status) { WeaponStatus = Status; }

protected:
	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComponent;

	// 武器のメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	// 武器のステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FStatus WeaponStatus;

	// 武器の属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	EElement WeaponElement;

};
