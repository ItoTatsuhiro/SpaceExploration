// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/SelectPlanetPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include <EnhancedInputSubsystems.h>
#include "InputAction.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Scene/SelectTileScene/AC_StageMapManager.h"

void ASelectPlanetPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) {
		UE_LOG(LogTemp, Log, TEXT("InputComponet が空です"));
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInputComponent) {
		UE_LOG(LogTemp, Log, TEXT("EnhancedInputComponent のキャストに失敗しました"));
		return;
	}

	EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &ASelectPlanetPlayerController::LeftClickEvent);

	// InputMapping Context を登録する
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())
		)
	{
		Subsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
	}
}

void ASelectPlanetPlayerController::LeftClickEvent()
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("左クリック時のイベントが実行されました。")), true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogClass, Log, TEXT("左クリック時のイベントが実行されました。"));

	if (!StageMapManagerRef)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASelectPlanetPlayerController:StageMapManagerが空です。")), true, true, FColor::Yellow, 2.f, TEXT(""));
		UE_LOG(LogClass, Warning, TEXT("ASelectPlanetPlayerController:StageMapManagerが空です。"));
		return;
	}

	// ターンマネージャーに通知を行う
	StageMapManagerRef->OnClickInput();

}
