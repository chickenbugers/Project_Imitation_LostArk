// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LM_PC_BaseAnim.generated.h"

/**
 * 
 */
class ALM_Character_Base;
class UCharacterMovementComponent;

UCLASS()
class LOSTARKIMITATION_API ULM_PC_BaseAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bShouldMove;

	// State 추가 예정
	// 점프 키 없을 예정

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<ALM_Character_Base> OwnerPlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> OwnerPlayerMovementComponent;
};
