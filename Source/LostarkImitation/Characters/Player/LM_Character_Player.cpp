// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/LM_Character_Player.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"

// (옵션) 아래 컴포넌트들은 실제로 구현해야 하며, 프로젝트에 없으면 주석 처리 가능
//#include "AttributeComponent.h"
//#include "SkillComponent.h"
//#include "EquipmentComponent.h"

ALM_Character_Player::ALM_Character_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root은 Character의 CapsuleComponent가 기본으로 있음.

	// SpringArm 생성 및 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent()); // 캡슐에 붙임 (캐릭터 중심)
	SpringArm->TargetArmLength = DefaultArmLength;
	SpringArm->bUsePawnControlRotation = false; // Pawn Control Rotation을 상속하지 않음 (로스트아크 스타일)
	SpringArm->bEnableCameraLag = bEnableCameraLag;
	SpringArm->CameraLagSpeed = CameraLagSpeed;
	SpringArm->SetRelativeRotation(FRotator(DefaultArmPitch, 0.f, 0.f));
	SpringArm->bDoCollisionTest = true; // 충돌 처리 (지형에 가려지는 것 방지)

	// Camera 생성 및 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 예시 컴포넌트들 (프로젝트에 실제 구현체가 있다면 활성화)
	//AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	//SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	//EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));

	// Movement 기본 설정 (모바일/액션에 맞춰 보정)
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;

	// 캐릭터 전용 Movement 캐시 (BeginPlay에서 다시 안전하게 세팅)
	LMCharacterMovement = nullptr;

	// 기본 캔슬 등 설정 (필요시 확장)
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ALM_Character_Player::BeginPlay()
{
	Super::BeginPlay();

	InitializeComponents();
}

void ALM_Character_Player::InitializeComponents()
{
	// SpringArm 초기 값 적용 (에디터에서 오버라이드 가능)
	if (SpringArm)
	{
		SpringArm->TargetArmLength = DefaultArmLength;
		SpringArm->SetRelativeRotation(FRotator(DefaultArmPitch, 0.f, 0.f));
		SpringArm->bEnableCameraLag = bEnableCameraLag;
		SpringArm->CameraLagSpeed = CameraLagSpeed;
	}
}


void ALM_Character_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 모바일 전용 카메라 보정 또는 부드러운 이동 처리
	UpdateCameraForMobile(DeltaTime);

	// PendingMoveInput을 사용하여 별도 처리 필요 시 여기에 구현
	if (!PendingMoveInput.IsNearlyZero())
	{
		// 예: 애니메이션 블렌드 스페이스용 Speed 계산 등
	}
}

void ALM_Character_Player::UpdateCameraForMobile(float DeltaTime)
{
	// 필요 시 카메라를 플레이어 속도/상태에 맞춰 소폭 보정하는 로직을 넣어라.
	// 예: 전투 중 카메라 확대/축소, 스킬 시 카메라 흔들림 등
}

void ALM_Character_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
}

#pragma region Auto Move
void ALM_Character_Player::MoveToLocation(const FVector& InTarget)
{
	StopDirectionalMove();
	TargetLocation = InTarget;

	GetWorldTimerManager().SetTimer(
		AutoMoveHandle, this,
		&ALM_Character_Player::AutoMoveStep,
		0.016f, true
	);
}

void ALM_Character_Player::AutoMoveStep()
{
	FVector Pos = GetActorLocation();
	FVector Delta = TargetLocation - Pos;
	Delta.Z = 0;

	if (Delta.Size() < 20.f)
	{
		StopAutoMove();
		return;
	}

	FVector Dir = Delta.GetSafeNormal();
	float Speed = GetCharacterMovement()->MaxWalkSpeed;
	FVector MoveDelta = Dir * Speed * 0.016f;

	FHitResult Hit;
	GetCharacterMovement()->SafeMoveUpdatedComponent(
		MoveDelta, GetActorRotation(), true, Hit
	);
}

void ALM_Character_Player::StopAutoMove()
{
	GetWorldTimerManager().ClearTimer(AutoMoveHandle);
}
#pragma endregion


#pragma region Hold Move
void ALM_Character_Player::MoveByDirection(const FVector2D& Dir)
{
	StopAutoMove();

	FRotator CamRot = GetControlRotation();
	FVector Forward = CamRot.Vector(); Forward.Z = 0; Forward.Normalize();
	FVector Right = FRotationMatrix(CamRot).GetUnitAxis(EAxis::Y);

	HoldDirection = (Forward * Dir.Y + Right * Dir.X).GetSafeNormal();

	if (!GetWorldTimerManager().IsTimerActive(HoldMoveHandle))
	{
		GetWorldTimerManager().SetTimer(
			HoldMoveHandle, this,
			&ALM_Character_Player::HoldMoveStep,
			0.016f, true
		);
	}
}

void ALM_Character_Player::HoldMoveStep()
{
	if (HoldDirection.IsNearlyZero()) return;

	float Speed = GetCharacterMovement()->MaxWalkSpeed * 0.6f;
	FVector MoveDelta = HoldDirection * Speed * 0.016f;

	FHitResult Hit;
	GetCharacterMovement()->SafeMoveUpdatedComponent(
		MoveDelta, GetActorRotation(), true, Hit
	);
}

void ALM_Character_Player::StopDirectionalMove()
{
	GetWorldTimerManager().ClearTimer(HoldMoveHandle);
	HoldDirection = FVector::ZeroVector;
}
#pragma endregion