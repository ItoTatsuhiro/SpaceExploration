// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include <EnhancedInputSubsystems.h>
#include "InputAction.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Camera/BaseCamera.h"
#include "Weapon/WeaponInventoryComponent.h"
#include "Weapon/WeaponBase.h"
#include "GameFramework/PlayerController.h"
#include "Character/MouseButtonEvent.h"
#include "NiagaraComponent.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/GameplayStatics.h"


APlayerCharacter::APlayerCharacter() : TargetLocation({ 0, 0, 0 }), IdleAngle(0), MoveSpeed(2000.0f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogClass, Log, TEXT("PlayerCharacterのコンストラクタが実行されました。"));

	CharacterStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayeMesh"));

	CharacterStaticMeshComp->SetupAttachment(RootComponent);

	PlayerSequence.BindUObject(this, &APlayerCharacter::SeqIdle);

	// ----------- カメラの作成 -----------------------------------------------------
	LookingDownCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("LookingDownCameraSpringArm"));
	LookingDownCameraSpringArm->SetMobility(EComponentMobility::Movable);
	LookingDownCameraSpringArm->SetupAttachment(RootComponent);

	LookingDownCameraSpringArm->TargetArmLength = 1000.f;
	LookingDownCameraSpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	LookingDownCameraComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("LookingDownCameraComponent"));
	LookingDownCameraComp->SetChildActorClass(ABaseCamera::StaticClass());
	LookingDownCameraComp->SetupAttachment(LookingDownCameraSpringArm);

	// ----------- ナイアガラの初期化 ------------------------------------------------

	JetNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("JetNiagara"));
	JetNiagaraComp->SetMobility(EComponentMobility::Movable);
	JetNiagaraComp->SetupAttachment(RootComponent);

	DamageNiagaraComp->SetActive(false);
	DeathNiagaraComp->SetActive(false);


}

/// <summary>
/// プレイヤー生成時に一度だけ実行する
/// </summary>
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	JetNiagaraComp->Activate(true);

	// データテーブルをロード
	PlayerDataTable.LoadSynchronous();

	// プレイヤーのステータスを設定
	SetStatusForDataTable(1);

	// PlayerController の取得
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayCharacter::BeginPlay() : PlayerControllerの取得に失敗しました。"));
		return;
	}
	// マウスカーソルを表示
	PlayerController->bShowMouseCursor = true;

	PlayerController->SetViewTargetWithBlend(LookingDownCameraComp->GetChildActor());

	// プレイヤーのトランスフォームを設定
	const APlayerStart* PlayerStart = Cast<APlayerStart>( UGameplayStatics::GetActorOfClass( GetWorld(), APlayerStart::StaticClass() ) );

	if (!PlayerStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayCharacter::BeginPlay() : PlayerStartの取得に失敗しました。"));
		return;
	}

	SetActorTransform(PlayerStart->GetActorTransform());

	DefaultPlayerLocation = PlayerStart->GetActorLocation();
	DefaultPlayerRotate = PlayerStart->GetActorRotation();
	DefaultLookingDownCameraRotate = LookingDownCameraSpringArm->GetRelativeRotation();

	//// マウスカーソルのモードを UI モードに設定 (必要に応じて)
	//FInputModeUIOnly InputMode;
	//PlayerController->SetInputMode(InputMode);

}

//　----------------------------------------------------------------
// プレイヤーのアップデート
//　---------------------------------------------------------------->
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerSequence.IsBound()) {
		UE_LOG(LogTemp, Error, TEXT("APlayCharacter:Don,t set PlayerSequence function"), nullptr);
		return;
	}

	PlayerSequence.Execute(DeltaTime);
	SequenceElapsedTime += DeltaTime;
}

// --------------------------------------------------------------------------
// 攻撃処理を行う
// --------------------------------------------------------------------------
void APlayerCharacter::Attack()
{
	if (!EquippedWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::Attack() : EquippedWeapon が nullptr でした"));
		return;
	}

	E_CharacterActState = ECharacterActState::Attack;
	PlayerSequence.BindUObject(this, &APlayerCharacter::SeqAttack);
	SequenceElapsedTime = 0.0f;
	EquippedWeapon->ExecuteAttack();

	UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::Attack() : 攻撃開始"));
}

// --------------------------------------------------------------------------
// ダメージを受ける処理を行う
// 
// ・引数
// Damage：ダメージ量
// --------------------------------------------------------------------------
void APlayerCharacter::TakeDamage(int32 Damage)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("プレイヤーは%dダメージ受けた"), Damage), true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::TakeDamage() : プレイヤーは%dダメージ受けた"), Damage);
	CharacterStatus.HP -= Damage;
	E_CharacterActState = ECharacterActState::TakeDamage;

	SequenceElapsedTime = 0.0f;
	PlayerSequence.BindUObject(this, &APlayerCharacter::SeqTakeDamage);
	DamageNiagaraComp->Activate();
}

// --------------------------------------------------------------------------
// 死亡時の処理を行う
// --------------------------------------------------------------------------
void APlayerCharacter::Death()
{
	// メッシュを非表示にする
	CharacterStaticMeshComp->SetHiddenInGame(true);
	EquippedWeapon->SetActorHiddenInGame(true);
	
	// 噴射演出を非表示
	JetNiagaraComp->Deactivate();
	JetNiagaraComp->SetHiddenInGame(true);

	DeathNiagaraComp->Activate();
	SequenceElapsedTime = 0.0f;
	E_CharacterActState = ECharacterActState::Death;
	PlayerSequence.BindUObject(this, &APlayerCharacter::SeqDeath);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("プレイヤーはやられた")), true, true, FColor::Cyan, 2.f, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::Death() : プレイヤーはやられた"));
}

void APlayerCharacter::SetCharacterLocation(const FVector& Location)
{
	ACharacterBase::SetCharacterLocation(Location);

	DefaultPlayerLocation = Location;
}

AWeaponBase* APlayerCharacter::GetElementWeapon(EElement WeaponElement)
{
	return nullptr;
}

// ----------------------------------------------------------------
// 移動させたい位置をセットさせて、移動を開始させる。
// ----------------------------------------------------------------
void APlayerCharacter::BeginMoveTargetLocation(const FVector& Location)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Player Move TargetLocation：x = %1.f, y = %1.f, z = %1.f"), 
		Location.X, Location.Y, Location.Z )
		, true, true, FColor::Cyan, 2.f, TEXT(""));

	FVector ActorLocation = GetActorLocation();

	TargetLocation = Location;

	// アクターの角度を調整
	FVector Distance = GetActorLocation() - TargetLocation;

	float Angle = FMath::RadiansToDegrees( FMath::Atan2(Distance.Y, Distance.X) ) - 90.f;

	FRotator ActorRotation = GetActorRotation();
	FRotator NewRotation = FRotator(ActorRotation.Pitch, Angle, ActorRotation.Roll);

	SetActorRotation(NewRotation);
	LookingDownCameraSpringArm->SetRelativeRotation(FRotator(DefaultLookingDownCameraRotate.Pitch, DefaultPlayerRotate.Yaw - Angle, DefaultLookingDownCameraRotate.Roll ) );

	// 目標位置にに移動するシーケンスにデリゲートを切り替える
	PlayerSequence.BindUObject(this, &APlayerCharacter::SeqMoveTargetLocation);

	E_CharacterActState = ECharacterActState::Move;
}

// -----------------------------------------------------------------
// プレイヤーの経験値を追加。
// -----------------------------------------------------------------
void APlayerCharacter::AddExp(int Exp)
{
	CharacterStatus.Exp += Exp;
}

// -----------------------------------------------------------------
// レベルが上げられるか判定を返す。
// 
// args1...追加する経験値
// -----------------------------------------------------------------
bool APlayerCharacter::CanLevelUp()
{
	if (!PlayerDataTable)
	{
		UE_LOG(LogClass, Error, TEXT("APlayerCharacter::CanLevelUp() : PlayerDataTable がセットされていません"));
		return false;
	}

	TArray<FName> Names = PlayerDataTable->GetRowNames();

	if (CharacterStatus.PlayerLevel < 0 || CharacterStatus.PlayerLevel >= Names.Num())
	{
		UE_LOG(LogClass, Warning, TEXT("APlayerCharacter::CanLevelUp() : %d は PlayerDataTable の行の範囲外です。"), CharacterStatus.PlayerLevel);
		return false;
	}

	FStatus* NextStatus = PlayerDataTable->FindRow<FStatus>(Names[CharacterStatus.PlayerLevel], FString());

	if (!NextStatus)
	{
		UE_LOG(LogClass, Error, TEXT("APlayerCharacter::CanLevelUp() : PlayerDataTable に FStatus型のデータテーブルが存在しませんでした"));
		return false;
	}

	if ( CharacterStatus.Exp < NextStatus->Exp )
	{
		UE_LOG(LogClass, Log, TEXT("APlayerCharacter::CanLevelUp() : レベルアップに必要な経験値に達していません...現在経験値：%d, 必要経験値：%d"), CharacterStatus.Exp, NextStatus->Exp);
		return false;
	}

	UE_LOG(LogClass, Log, TEXT("APlayerCharacter::CanLevelUp() : レベルアップ可能...現在経験値：%d, 必要経験値：%d"), CharacterStatus.Exp, NextStatus->Exp);

	return true;
}

// -----------------------------------------------------------------
// プレイヤーのレベルアップを実行する
// -----------------------------------------------------------------
void APlayerCharacter::ExecuteLevelUp()
{
	SetStatusForDataTable(CharacterStatus.PlayerLevel + 1);

	UE_LOG(LogClass, Log, TEXT("APlayerCharacter::ExecuteLevelUp() : レベルアップ完了") );
}

// -----------------------------------------------------------------
// 指定したレベルのプレイヤーのステータスをセットする
// 
// SetLevel...セットするステータスのレベル
// -----------------------------------------------------------------
void APlayerCharacter::SetStatusForDataTable(int SetLevel)
{
	SetLevel -= 1;

	if (!PlayerDataTable)
	{
		UE_LOG(LogClass, Error, TEXT("APlayerCharacter::SetStatusForDataTable() : PlayerDataTable がセットされていません"));
		return;
	}

	TArray<FName> Names = PlayerDataTable->GetRowNames();

	if (SetLevel < 0 || SetLevel >= Names.Num())
	{
		UE_LOG(LogClass, Warning, TEXT("APlayerCharacter::SetStatusForDataTable() : %d は PlayerDataTable の行の範囲外です。"), SetLevel);
		return;
	}

	FStatus* NextStatus = PlayerDataTable->FindRow<FStatus>(Names[SetLevel], FString());

	if (!NextStatus)
	{
		UE_LOG(LogClass, Error, TEXT("APlayerCharacter::SetStatusForDataTable() : PlayerDataTable に FStatus型のデータテーブルが存在しませんでした"));
		return;
	}

	CharacterStatus.PlayerLevel = NextStatus->PlayerLevel;
	CharacterStatus.MaxHp = NextStatus->MaxHp;
	CharacterStatus.HP = CharacterStatus.MaxHp;
	CharacterStatus.AttackPower = NextStatus->AttackPower;
	CharacterStatus.DefencePower = NextStatus->DefencePower;
	CharacterStatus.Speed = NextStatus->Speed;

}

//　----------------------------------------------------------------
// 左クリックを押したときレイを飛ばして
// 当たった "Actor" クラスの "LeftMouseButton" 関数を実行する
// ----------------------------------------------------------------
void APlayerCharacter::ClickedMouseLeftButton()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UKismetSystemLibrary::PrintString(this, "APlayCharacter:PlayerControllerの取得に失敗しました。", true, true, FColor::Red, 2.f, TEXT(""));
		UE_LOG(LogTemp, Warning, TEXT("APlayCharacter:PlayerControllerの取得に失敗しました。"), nullptr);
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
				UE_LOG(LogTemp, Warning, TEXT("APlayCharacter:Actorの取得に失敗しました。"));
				return;
			}

			// インターフェイスが存在する場合、左クリック時のイベント関数を呼び出す。
			IMouseButtonEvent::Execute_LeftMouseButtonEvent(HitActor, this);

			// Debug line to visualize the trace
			// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
		}
	}
}

//　----------------------------------------------------------------
// 待機シーケンス
// 
// == 引数 ===
// DeltaTime...
// 
// 実行結果を返す。False：失敗 True：成功
// ----------------------------------------------------------------
bool APlayerCharacter::SeqIdle(const float DeltaTime)
{
	IdleAngle += 2;
	IdleAngle %= 360;

	/*SetActorLocation(
		FVector(
			DefaultPlayerLocation.X,
			DefaultPlayerLocation.Y,
			DefaultPlayerLocation.Z + ( FMath::Sin( FMath::DegreesToRadians(IdleAngle) ) * 100.f ) 
		) 
	);*/

	return true;
}

// ----------------------------------------------------------------
// プレイヤーを目標位置に移動させる。
// 移動が終了したら "SeqIdle" に戻る。
//
// 戻り値：実行結果を返す。False：失敗 True：成功
// ----------------------------------------------------------------
bool APlayerCharacter::SeqMoveTargetLocation(const float DeltaTime)
{
	// ターゲットまでの方向ベクトルを計算
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();

	// 移動速度を方向に適用
	FVector MoveDirection = Direction * MoveSpeed * DeltaTime;

	// プレイヤーの位置を更新
	FVector NewLocation = GetActorLocation() + MoveDirection;
	SetActorLocation(NewLocation);

	if (FVector::Dist(GetActorLocation(), TargetLocation) <= 100.f) {
		SetActorLocation(TargetLocation);
		
		SetActorRelativeRotation(DefaultPlayerRotate);
		LookingDownCameraSpringArm->SetRelativeRotation(DefaultLookingDownCameraRotate);

		PlayerSequence.BindUObject(this, &APlayerCharacter::SeqIdle);
		E_CharacterActState = ECharacterActState::Idle;
	}
	return true;
}

// ----------------------------------------------------------------
// プレイヤーの攻撃処理を実行する。
// 攻撃が終了または失敗したら "SeqIdle" に戻る。
//
// 戻り値：実行結果を返す。False：失敗 True：成功
// ----------------------------------------------------------------
bool APlayerCharacter::SeqAttack(const float DeltaTime)
{
	if (!EquippedWeapon)
	{
		UE_LOG(LogClass, Log, TEXT("EquippedWeapon is nullptr"));
		E_CharacterActState = ECharacterActState::Idle;
		PlayerSequence.BindUObject(this, &APlayerCharacter::SeqIdle);
		return false;
	}

	if (!EquippedWeapon->IsAttack())
	{
		UE_LOG(LogClass, Log, TEXT("APlayerCharacter::SeqAttack() : WeaponAttack is not Active"));
		E_CharacterActState = ECharacterActState::Idle;
		PlayerSequence.BindUObject(this, &APlayerCharacter::SeqIdle);
	}

	return true;
}

// ----------------------------------------------------------------
// プレイヤーの攻撃処理を実行する。
// 攻撃が終了し、HPが 0 だったら "SeqDeath" に変更する
// そうでない場合、"SeqIdle" に戻る。
//
// 戻り値：実行結果を返す。False：失敗 True：成功
// ----------------------------------------------------------------
bool APlayerCharacter::SeqTakeDamage(const float DelataTime)
{
	if ( DamageNiagaraComp->IsActive() )
	{
		return true;
	}

	// HPが 0 の場合、死亡演出を行う
	if (CharacterStatus.HP <= 0)
	{
		Death();
		return true;
	}

	UE_LOG(LogClass, Log, TEXT("ダメージ演出終了"));
	E_CharacterActState = ECharacterActState::Idle;
	PlayerSequence.BindUObject(this, &APlayerCharacter::SeqIdle);

	return true;
}

// ----------------------------------------------------------------
// プレイヤーの攻撃処理を実行する。
// 攻撃が終了または失敗したら "SeqIdle" に戻る。
//
// 戻り値：実行結果を返す。False：失敗 True：成功
// ----------------------------------------------------------------
bool APlayerCharacter::SeqDeath(const float DeltaTime)
{
	if (SequenceElapsedTime < DeathEffectTime)
	{
		return true;
	}

	UE_LOG(LogClass, Log, TEXT("死亡時の演出終了"));
	E_CharacterActState = ECharacterActState::Idle;
	PlayerSequence.BindUObject(this, &APlayerCharacter::SeqIdle);

	return true;
}


