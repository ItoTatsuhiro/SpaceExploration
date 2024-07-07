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
	
	// ����̈ʒu���擾����
	inline const FVector3f& GetWeaponLocation() const { return WeaponLocation; }
	// ����̃X�e�[�^�X���擾����
	inline const FStatus& GetWeaponStatus() const { return WeaponStatus; }
	//// ����̑������擾����
	//inline const EElement GetWeaponElement() const { return WeaponElement; }

	// ==========================================================================
	// ��������
	// ==========================================================================
	// ����̈ʒu��ݒ肷��
	inline void SetWeaponLocation(const FVector3f& Location) { WeaponLocation = Location; }
	// inline void SetWeaponStatus(const FStatus& Status) { WeaponStatus = Status; }

	// ����̃X�e�[�^�X��ݒ肷��
	void SetWeaponStatus(const FStatus& Status);

protected:
	// ����̈ʒu
	UPROPERTY(EditAnywhere)
	FVector3f WeaponLocation;

	// ����̃X�e�[�^�X
	UPROPERTY(EditAnywhere)
	FStatus WeaponStatus;

	//// ����̑���
	//UPROPERTY(EditAnywhere)
	//EElement WeaponElement;

};
