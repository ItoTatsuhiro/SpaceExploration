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
	WeaponAttackNiagaraComp->SetupAttachment(MeshComponent);

}
// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	ExecuteAttack();

}

void AWeaponBase::SettingWeapon()
{
		UE_LOG(LogTemp, Warning, TEXT("Set"));
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (PlayerCharacter)
		{
			AWeaponBase* EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
			if (!EquippedWeapon)
			{
				NextLevelStatus = CurrentLevelStatus;
				UE_LOG(LogTemp, Error, TEXT("武器が空です"));
				return;
			}
				if (EquippedWeapon->WeaponElement == WeaponElement)
				{
					NextLevelStatus.PlayerName   = CurrentLevelStatus.PlayerName;
					NextLevelStatus.PlayerLevel  = CurrentLevelStatus.PlayerLevel + 1;
					NextLevelStatus.HP           = CurrentLevelStatus.HP + ((NextLevelStatus.PlayerLevel - 1) * HpEnhancedValue);
					NextLevelStatus.Speed        = CurrentLevelStatus.Speed + ((NextLevelStatus.PlayerLevel - 1) * SpeedEnhancedValue);
					NextLevelStatus.AttackPower  = CurrentLevelStatus.AttackPower + ((NextLevelStatus.PlayerLevel - 1) * AttackEnhancedValue);
					NextLevelStatus.DefencePower = CurrentLevelStatus.DefencePower + ((NextLevelStatus.PlayerLevel - 1) * DeffenceEnhancedValue);
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

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AWeaponBase::LeftMouseButtonEvent_Implementation(APlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) {
		UKismetSystemLibrary::PrintString(this, "PlayerCharacter is nullptr", true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is nullptr"));
		return;
	}
	CurrentLevelStatus = NextLevelStatus;
	PlayerCharacter->SetEquippedWeapon(this);
	UWorld* World = GetWorld();
	ALevelScriptActor* LevelScript = World->GetLevelScriptActor();
	if (LevelScript)
	{
		FName FunctionName(TEXT("Weapon Sets")); 
		UFunction* Function = LevelScript->FindFunction(FunctionName);

		if (Function)
		{
			LevelScript->ProcessEvent(Function, nullptr);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("WeaponSets function not found"));
		}
	}
	Destroy();
}