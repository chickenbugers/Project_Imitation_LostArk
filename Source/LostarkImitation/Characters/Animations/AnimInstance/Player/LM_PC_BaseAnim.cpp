// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animations/AnimInstance/Player/LM_PC_BaseAnim.h"

#include "Characters/Player/LM_Character_Base.h"
#include "GameFramework/CharacterMovementComponent.h"


void ULM_PC_BaseAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Is Valid - TryGetPawnOwner
	if (!TryGetPawnOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("ULM_PC_BaseAnim::NativeInitializeAnimation() - Pawn Owner is null."));
		return;
	}

	// Is Valid - OwnerPlayerCharacter
	OwnerPlayerCharacter = Cast<ALM_Character_Base>(TryGetPawnOwner());
	if (!OwnerPlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ULM_PC_BaseAnim::NativeInitializeAnimation() - OwnerPlayerCharacter is null."));
		return;
	}

	OwnerPlayerMovementComponent = OwnerPlayerCharacter->GetCharacterMovement();
}

void ULM_PC_BaseAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerPlayerMovementComponent)
		return;

	// Velocity Update
	this->Velocity = OwnerPlayerMovementComponent->Velocity;

	// Speed Update (Z축 제외)
	GroundSpeed = Velocity.Size2D();

	// bShouldMove Update -> True :: 움직임(WALK), False :: 정지(IDLE)
	bShouldMove = (OwnerPlayerMovementComponent->GetCurrentAcceleration() != FVector::ZeroVector) && (GroundSpeed > 3.0f);
}
