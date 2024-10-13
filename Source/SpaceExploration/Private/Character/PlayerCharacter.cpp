// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include <EnhancedInputSubsystems.h>
#include "InputAction.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/BaseCamera.h"
#include "Weapon/WeaponInventoryComponent.h"
#include "Weapon/WeaponBase.h"
#include "GameFramework/PlayerController.h"
#include "Character/MouseButtonEvent.h"
#include <Kismet/KismetSystemLibrary.h>


APlayerCharacter::APlayerCharacter() : TargetLocation({ 0, 0, 0 }), MoveSpeed(2000.0f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogClass, Log, TEXT("PlayerCharacterのコンストラクタが実行されました。"), nullptr);

	CharacterStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayeMesh"));

	CharacterStaticMeshComp->SetupAttachment(RootComponent);

	PlayerSequence.BindUObject(this, &APlayerCharacter::SeqIdle);

	// ----------- カメラの作成 -----------------------------------------------------
	LookingDownCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("LookingDownCameraSpringArm"));
	LookingDownCameraSpringArm->SetupAttachment(CharacterStaticMeshComp);

	LookingDownCameraSpringArm->TargetArmLength = 1000.f;
	LookingDownCameraSpringArm->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));

	LookingDownCameraComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("LookingDownCameraComponent"));
	LookingDownCameraComp->SetChildActorClass(ABaseCamera::StaticClass());
	LookingDownCameraComp->SetupAttachment(LookingDownCameraSpringArm);

	// ----------- 武器インベントリコンポーネントの生成 ------------------------------
	WeaponInventoryComponent = CreateDefaultSubobject<UWeaponInventoryComponent>(TEXT("Weapon Inventory"));

	if (!WeaponInventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponInventoryの生成に失敗しました。"));
	}

	ClickedEvnet = nullptr;
}

/// <summary>
/// プレイヤー生成時に一度だけ実行する
/// </summary>
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// PlayerController の取得
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UKismetSystemLibrary::PrintString(this, "PlayerControllerの取得に失敗しました。", true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogTemp, Warning, TEXT("PlayerControllerの取得に失敗しました。"));
		return;
	}
	// マウスカーソルを表示
	PlayerController->bShowMouseCursor = true;

	PlayerController->SetViewTargetWithBlend(LookingDownCameraComp->GetChildActor());

	if (!WeaponInventoryComponent) {
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("WeaponInventoryの取得に失敗しました。")), true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogTemp, Error, TEXT("WeaponInventoryの取得に失敗しました。"));
		return;
	}

	WeaponInventoryComponent->CreateElememtWeapons();
	
	//// マウスカーソルのモードを UI モードに設定 (必要に応じて)
	//FInputModeUIOnly InputMode;
	//PlayerController->SetInputMode(InputMode);

}

/// <summary>
/// プレイヤーの入力イベントのセットアップを行う
/// </summary>
/// <param name="PlayerInputComponent"> 入力マッピングコンテキスト </param>
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
	if (!GetOwner()->InputComponent)
	{
		UE_LOG(LogClass, Error, TEXT("EnhancedInputComponentがNullです2"));
		return;
	}
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (!PlayerController)
	{
		return;
	}

	// InputMapping Context を登録する
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())
		)
	{
		Subsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
	}
}

/// <summary>
/// プレイヤーのアップデート
/// </summary>
/// <param name="DeltaTime"></param>
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerSequence.IsBound()) {
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Don,t set PlayerSequence function")), true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogTemp, Error, TEXT("Don,t set PlayerSequence function"), nullptr);
		return;
	}

	PlayerSequence.Execute(DeltaTime);
}

/// <summary>
/// 移動させたい位置をセットさせて、移動を開始させる。
/// </summary>
/// <param name="Location"> 移動させる位置 </param>
void APlayerCharacter::BeginMoveTargetLocation(const FVector Location)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Player Move TargetLocation：x = %1.f, y = %1.f, z = %1.f"), 
		Location.X, Location.Y, Location.Z )
		, true, true, FColor::Cyan, 2.f, TEXT(""));
	TargetLocation = Location;
	PlayerSequence.BindUObject(this, &APlayerCharacter::SeqMoveTargetLocation);
}

//
// 左クリックを押したときレイを飛ばして
// 当たった "Actor" クラスの "LeftMouseButton" 関数を実行する
// 
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
				UE_LOG(LogTemp, Warning, TEXT("Actorの取得に失敗しました。"));
				return;
			}

			// インターフェイスが存在する場合、左クリック時のイベント関数を呼び出す。
			IMouseButtonEvent::Execute_LeftMouseButtonEvent(HitActor, this);

			// Debug line to visualize the trace
			// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
		}
	}
}

/// <summary>
/// 待機シーケンス
/// </summary>
/// <param name="DeltaTime"></param>
/// <returns> 実行結果を返す。False：失敗 True：成功 </returns>
bool APlayerCharacter::SeqIdle(float DeltaTime)
{
	return true;
}

// プレイヤーを目標位置に移動させる。
// 移動が終了したら "SeqIdle" に戻る。
//
// 戻り値：実行結果を返す。False：失敗 True：成功 </returns>
bool APlayerCharacter::SeqMoveTargetLocation(float DeltaTime)
{
	// ターゲットまでの方向ベクトルを計算
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();

	// 移動速度を方向に適用
	FVector MoveDirection = Direction * MoveSpeed * DeltaTime;

	// プレイヤーの位置を更新
	FVector NewLocation = GetActorLocation() + MoveDirection;
	SetActorLocation(NewLocation);

	if (FVector::Dist(GetActorLocation(), TargetLocation) <= 20.f) {
		SetActorLocation(TargetLocation);
		PlayerSequence.BindUObject(this, &APlayerCharacter::SeqIdle);
	}
	return true;
}


