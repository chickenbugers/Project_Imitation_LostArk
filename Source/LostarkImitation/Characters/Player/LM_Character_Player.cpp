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
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	/* ===== Rotation ===== */
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/* ===== Movement ===== */
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	/* ===== Camera ===== */
	// Create the camera boom component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = 800.f;
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->bDoCollisionTest = false;

	// Create the camera component
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	/* ===== Tick ===== */
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	/* ====== Replication ===== */
	GetCharacterMovement()->SetIsReplicated(true);
	SetReplicatingMovement(true);
	bReplicates = true;

	/* ===== Component ===== */
	LMCharacterMovement = nullptr;
}

void ALM_Character_Player::BeginPlay()
{
	Super::BeginPlay();

	InitializeComponents();
	CacheMovementComponent();
}

void ALM_Character_Player::CacheMovementComponent()
{
	LMCharacterMovement = GetCharacterMovement();
}

void ALM_Character_Player::Server_MoveToLocation_Implementation(const FVector& Dest)
{
	if (!HasAuthority())
		return;

	AController* control = GetController();
	if (!control) 
		return;

	FVector Dir = Dest - GetActorLocation();
	Dir.Z = 0.0f;

	if (Dir.IsNearlyZero())
		return;

	const FRotator Rot = Dir.Rotation();

	// 회전만 멀티캐스트
	Multicast_SetRotation(FRotator(0.f,Rot.Yaw,0.f));

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(
		control,
		Dest
	);
}

void ALM_Character_Player::Multicast_SetRotation_Implementation(const FRotator& Rot)
{
	// AutonomousProxy 포함, 전부 적용
	SetActorRotation(Rot);
}