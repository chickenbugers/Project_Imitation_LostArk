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

protected:
	/** Tick */
	virtual void Tick(float DeltaTime) override;

	/** BeginPlay */
	virtual void BeginPlay() override;

	/** Setup Player Input (Enhanced Input 사용 시 확장 가능) */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/* ================= 이동 인터페이스 ================= */

	/** 클릭 이동 (AI MoveTo 기반) */
	void MoveToLocation(const FVector& InTarget);

	/** AI 이동 중단 */
	void StopAutoMove();

	/** 모바일 / WASD 방향 이동 */
	void MoveByDirection(const FVector2D& Dir);
	void StopDirectionalMove();

	/** 마우스 방향 홀드 이동 (PC) */
	void MoveTowardMouse(const FVector2D& MousePos);
	void StopHoldMove();

	UFUNCTION(Server, Reliable)
	void Server_MoveToLocation(const FVector& Dest);

protected:
	/* ================= Components ================= */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> LMCharacterMovement;

protected:
	/* ================= Camera Settings ================= */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings", meta = (ClampMin = "200.0", ClampMax = "3000.0"))
	float DefaultArmLength = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings", meta = (ClampMin = "-85.0", ClampMax = "-10.0"))
	float DefaultArmPitch = -55.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	bool bEnableCameraLag = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings", meta = (ClampMin = "0.0"))
	float CameraLagSpeed = 10.f;

protected:
	/* ================= Helpers ================= */

	void InitializeComponents();
	void CacheMovementComponent();
	void UpdateCameraForMobile(float DeltaTime);

private:
	/* ================= Hold Move ================= */

	FVector HoldDirection;
	FTimerHandle HoldMoveHandle;

	void HoldMoveStep();
};