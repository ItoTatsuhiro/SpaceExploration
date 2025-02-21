// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Character/PlayerCharacter.h"
#include "UObject/NoExportTypes.h"
#include "Math/RandomStream.h"
#include "random"
#include "NiagaraComponent.h"
#include "Engine/LevelScriptActor.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.                        
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent                = CreateDefaultSubobject<USceneComponent>(TEXT("Default Component"));
	RootComponent                 = SceneComponent;
	MeshComponent                 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	MeshComponent                 -> SetupAttachment(RootComponent);

	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);

	// 武器の攻撃エフェクトの設定
	WeaponAttackNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponAttackEffect"));
	WeaponAttackNiagaraComp->SetupAttachment(RootComponent);
	WeaponAttackNiagaraComp->SetActive(false);

}
// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::SettingWeapon()
{
		UE_LOG(LogTemp, Warning, TEXT("Set"));
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (PlayerCharacter)
		{
			AWeaponBase* PlayerWeapon = PlayerCharacter->GetElementWeapon(WeaponElement);
			if (!PlayerWeapon)
			{
				NextLevelStatus = CurrentLevelStatus;
				UE_LOG(LogTemp, Error, TEXT("武器が空です"));
				return;
			}
			if (PlayerWeapon->WeaponElement == WeaponElement)
			{
				NextLevelStatus.PlayerName   = CurrentLevelStatus.PlayerName;
				NextLevelStatus.PlayerLevel  = PlayerWeapon->CurrentLevelStatus.PlayerLevel + 1;
				NextLevelStatus.HP           = PlayerWeapon->CurrentLevelStatus.PlayerLevel + ((PlayerWeapon->CurrentLevelStatus.PlayerLevel - 1) * HpEnhancedValue);
				NextLevelStatus.Speed        = PlayerWeapon->CurrentLevelStatus.PlayerLevel + ((PlayerWeapon->CurrentLevelStatus.PlayerLevel - 1) * SpeedEnhancedValue);
				NextLevelStatus.AttackPower  = PlayerWeapon->CurrentLevelStatus.PlayerLevel + ((PlayerWeapon->CurrentLevelStatus.PlayerLevel - 1) * AttackEnhancedValue);
				NextLevelStatus.DefencePower = PlayerWeapon->CurrentLevelStatus.PlayerLevel + ((PlayerWeapon->CurrentLevelStatus.PlayerLevel - 1) * DeffenceEnhancedValue);
			}
			else {
				NextLevelStatus = CurrentLevelStatus;
			}
		}
		
}

// 武器の攻撃エフェクトを実行する
void AWeaponBase::ExecuteAttack()
{
	UE_LOG(LogTemp, Log, TEXT("AWeaponBase:Attack"));
	WeaponAttackNiagaraComp->Activate();
}

bool AWeaponBase::IsAttack()
{
	return WeaponAttackNiagaraComp->IsActive();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeaponBase::SetWeaponStatus(const FStatus& Status)
{
	FString WeaponName = CurrentLevelStatus.PlayerName;
	CurrentLevelStatus = Status;
	CurrentLevelStatus.PlayerName = WeaponName;
}

void AWeaponBase::LeftMouseButtonEvent_Implementation(APlayerCharacter* PlayerCharacter)
{
	
}

void AWeaponBase::SetWeaponMeshHiddenInGame(bool bNewHidden)
{
	MeshComponent->SetHiddenInGame(bNewHidden);
}
