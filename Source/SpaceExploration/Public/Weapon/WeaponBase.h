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
	
	// ����̃X�e�[�^�X���擾����
	inline const FStatus& GetWeaponStatus() const { return WeaponStatus; }
	// ����̑������擾����
	inline const EElement GetWeaponElement() const { return WeaponElement; }

	// ==========================================================================
	// ��������
	// ==========================================================================
	// ����̃X�e�[�^�X��ݒ肷��
	inline void SetWeaponStatus(const FStatus& Status) { WeaponStatus = Status; }

protected:
	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComponent;

	// ����̃��b�V��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	// ����̃X�e�[�^�X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	FStatus WeaponStatus;

	// ����̑���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	EElement WeaponElement;

};
