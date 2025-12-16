// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/LM_Character_Player.h"
#include "Core/PlayerController/LM_PlayerController.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Navigation/PathFollowingComponent.h"

#include "Net/UnrealNetwork.h"

ALM_Character_Player::ALM_Character_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	/* ===== Camera ===== */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = DefaultArmLength;
	SpringArm->bEnableCameraLag = bEnableCameraLag;
	SpringArm->CameraLagSpeed = CameraLagSpeed;
	SpringArm->SetRelativeRotation(FRotator(DefaultArmPitch, 0.f, 0.f));
	SpringArm->bDoCollisionTest = true;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->bUsePawnControlRotation = false;

	/* ===== Rotation ===== */ 
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	/* ===== Movement ===== */
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	GetCharacterMovement()->SetIsReplicated(true);
	SetReplicatingMovement(true);
	bReplicates = true;

	LMCharacterMovement = nullptr;
}

void ALM_Character_Player::BeginPlay()
{
	Super::BeginPlay();

	InitializeComponents();
	CacheMovementComponent();
}

void ALM_Character_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraForMobile(DeltaTime);
	if (HasAuthority())
	{
		FRotator CurrentRot = GetActorRotation();

		// 변화가 있을 때만 갱신 (네트워크 절약)
		if (!CurrentRot.Equals(ReplicatedRotation, 0.5f))
		{
			ReplicatedRotation = CurrentRot;
		}
	}
	else
	{
		// 클라 보간
		FRotator NewRot = FMath::RInterpTo(
			GetActorRotation(),
			TargetRotation,
			DeltaTime,
			12.f
		);
		SetActorRotation(NewRot);
	}
}

void ALM_Character_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
}

void ALM_Character_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALM_Character_Player, ReplicatedRotation);
}

/* ================= Click Move (AI) ================= */

void ALM_Character_Player::MoveToLocation(const FVector& InTarget)
{
	AController* PC = GetController();
	if (!PC)
		return;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(
		PC,
		InTarget
	);
}

void ALM_Character_Player::StopAutoMove()
{
	if (AController* PC = GetController())
	{
		PC->StopMovement();
	}
}

/* ================= Hold Move (PC / Mobile) ================= */

void ALM_Character_Player::MoveTowardMouse(const FVector2D& MousePos)
{
	StopAutoMove();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
		return;

	FVector WorldOrigin, WorldDir;
	PC->DeprojectScreenPositionToWorld(MousePos.X, MousePos.Y, WorldOrigin, WorldDir);

	float T = -(WorldOrigin.Z - GetActorLocation().Z) / WorldDir.Z;
	FVector TargetPos = WorldOrigin + WorldDir * T;

	FVector Dir = TargetPos - GetActorLocation();
	Dir.Z = 0.f;
	Dir.Normalize();

	AddMovementInput(Dir, 1.f);
}

void ALM_Character_Player::MoveByDirection(const FVector2D& Dir)
{
	StopAutoMove();

	FRotator CamRot = GetControlRotation();
	FVector Forward = CamRot.Vector();
	Forward.Z = 0.f;
	Forward.Normalize();

	FVector Right = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Y);

	HoldDirection = (Forward * Dir.Y + Right * Dir.X).GetSafeNormal();

	if (!GetWorldTimerManager().IsTimerActive(HoldMoveHandle))
	{
		GetWorldTimerManager().SetTimer(
			HoldMoveHandle,
			this,
			&ALM_Character_Player::HoldMoveStep,
			0.016f, true
		);
	}
}

void ALM_Character_Player::HoldMoveStep()
{
	if (HoldDirection.IsNearlyZero())
		return;

	float Speed = GetCharacterMovement()->MaxWalkSpeed * 0.6f;
	FVector MoveDelta = HoldDirection * Speed * 0.016f;

	FHitResult Hit;
	GetCharacterMovement()->SafeMoveUpdatedComponent(
		MoveDelta,
		GetActorRotation(),
		true,
		Hit
	);
}

void ALM_Character_Player::StopDirectionalMove()
{
	GetWorldTimerManager().ClearTimer(HoldMoveHandle);
	HoldDirection = FVector::ZeroVector;
}

void ALM_Character_Player::StopHoldMove()
{
	GetCharacterMovement()->StopMovementImmediately();
}

void ALM_Character_Player::OnRep_ServerRotation()
{
	TargetRotation = ReplicatedRotation;
}

void ALM_Character_Player::Server_MoveToLocation_Implementation(const FVector& Dest)
{
	UE_LOG(LogTemp, Warning,
		TEXT("Server_MoveToLocation | HasAuthority: %d | NetMode: %d"),
		HasAuthority(),
		(int32)GetWorld()->GetNetMode()
	);


	AController* control = GetController();
	if (!control) return;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(
		control,
		Dest
	);

	TargetRotation = ReplicatedRotation;
}

/* ================= Helpers ================= */

void ALM_Character_Player::InitializeComponents()
{
	if (SpringArm)
	{
		SpringArm->TargetArmLength = DefaultArmLength;
		SpringArm->SetRelativeRotation(FRotator(DefaultArmPitch, 0.f, 0.f));
		SpringArm->bEnableCameraLag = bEnableCameraLag;
		SpringArm->CameraLagSpeed = CameraLagSpeed;
	}
}

void ALM_Character_Player::CacheMovementComponent()
{
	LMCharacterMovement = GetCharacterMovement();
}

void ALM_Character_Player::UpdateCameraForMobile(float DeltaTime)
{
	// 모바일 전용 카메라 보정 확장 포인트
}