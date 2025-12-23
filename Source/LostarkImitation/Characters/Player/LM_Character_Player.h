// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/LM_Character_Base.h"
#include "LM_Character_Player.generated.h"

/**
 * 
 */
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UCharacterMovementComponent;
class AAIController;

UCLASS()
class LOSTARKIMITATION_API ALM_Character_Player : public ALM_Character_Base
{
	GENERATED_BODY()

public:
	ALM_Character_Player();

public:
	/* ===== Server ===== */
	UFUNCTION(Server, Reliable)
	void Server_MoveToLocation(const FVector& Dest);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetRotation(const FRotator& Rot);

protected:
	/** BeginPlay */
	virtual void BeginPlay() override;

protected:
	/* ================= Components ================= */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> LMCharacterMovement;

protected:
	/* ================= Helpers ================= */
	void CacheMovementComponent();
};