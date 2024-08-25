// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include <EnhancedInputSubsystems.h>
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Character/MouseButtonEvent.h"
#include <Kismet/KismetSystemLibrary.h>

/*
プレイヤーに左クリックのイベントを付けるとこから

たぶんカメラの実装できた。

*/


APlayerCharacter::APlayerCharacter() : TargetLocation({ 0, 0, 0 }), MoveSpeed(10.0f)
{
	CharacterStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayeMesh"));

	CharacterStaticMeshComp->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PlayerSequence.BindUObject(this, &APlayerCharacter::seqIdle);

	ClickedEvnet = nullptr;

}


void APlayerCharacter::BeginPlay()
{
	// PlayerController の取得
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UKismetSystemLibrary::PrintString(this, "PlayerControllerの取得に失敗しました。", true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogTemp, Warning, TEXT("PlayerControllerの取得に失敗しました。"), nullptr);
		return;
	}
	// マウスカーソルを表示
	PlayerController->bShowMouseCursor = true;

	//// マウスカーソルのモードを UI モードに設定 (必要に応じて)
	//FInputModeUIOnly InputMode;
	//PlayerController->SetInputMode(InputMode);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(ClickedEvnet, ETriggerEvent::Triggered, this, &ThisClass::ClickedMouseLeftButton);
	}
	else {
		UE_LOG(LogClass, Error, TEXT("EnhancedInputComponentがNullです"));
	}
	if (GetOwner()->InputComponent)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			// InputMapping Context を登録する
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())
				)
			{
				Subsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}
	}
	else {
		UE_LOG(LogClass, Error, TEXT("EnhancedInputComponentがNullです2"));
	}
}

void APlayerCharacter::MoveTargetLocation(const FVector Location)
{
	TargetLocation = Location;
	PlayerSequence.BindUObject(this, &APlayerCharacter::seqMoveTargetLocation);
}

/// <summary>
/// 左クリックを押したときレイを飛ばして
/// 当たった "Actor" クラスの "LeftMouseButton" を実行する
/// </summary>
void APlayerCharacter::ClickedMouseLeftButton()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UKismetSystemLibrary::PrintString(this, "PlayerControllerの取得に失敗しました。", true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogTemp, Warning, TEXT("PlayerControllerの取得に失敗しました。"), nullptr);
		return;
	}

	FVector WorldLocation, WorldDirection;

	// マウスの座標を取得
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {

		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f);

		FHitResult HitResult;

		// レイを飛ばす
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
		{
			AActor* HitActor = HitResult.GetActor();

			if ( !HitActor || !( HitActor->GetClass()->ImplementsInterface( UMouseButtonEvent::StaticClass() ) ) )
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Actorの取得に失敗しました。")), true, true, FColor::Yellow, 2.f, TEXT(""));
				UE_LOG(LogTemp, Warning, TEXT("Actorの取得に失敗しました。"), nullptr);
				return;
			}

			// MouseButtonEvent->LeftMouseButtonEvent(this);
			IMouseButtonEvent::Execute_LeftMouseButtonEvent(HitActor, this);

			// Debug line to visualize the trace
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
		}
	}
}

bool APlayerCharacter::seqIdle(float DeltaTime)
{
	return true;
}

bool APlayerCharacter::seqMoveTargetLocation(float DeltaTime)
{

	FVector MoveDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
	
	AddMovementInput(MoveDirection);

	if (FVector::Dist(GetActorLocation(), TargetLocation) <= 100.f) {
		PlayerSequence.BindUObject(this, &APlayerCharacter::seqIdle);
	}
	return true;
}


