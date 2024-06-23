// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterBase.generated.h"

UCLASS(Abstract)
class SPACEEXPLORATION_API ACharacterBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ダメージを受ける
	virtual void Move(int Damage) PURE_VIRTUAL(ACharacterBase::Move, );

	// 攻撃関数
	// UFunction(BlueprintCallable)
	virtual void Attack() PURE_VIRTUAL( ACharacterBase::Attack,);

	// 回復関数
	virtual void RecoverHP(int RecoveryAmount) PURE_VIRTUAL( ACharacterBase::RecoverHP, );

	// ダメージを受ける
	virtual void TakeDamage(int Damage) PURE_VIRTUAL( ACharacterBase::TakeDamage, );

private:
	// キャラクターの位置
	UPROPERTY(EditAnywhere)
	FVector3f CharacterLocation;

	// ステータス

	 // 装備品

};
