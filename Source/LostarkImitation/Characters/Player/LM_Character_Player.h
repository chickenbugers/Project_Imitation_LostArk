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
//class UAttributeComponent;
//class USkillComponent;
//class UEquipmentComponent;
class UCameraComponent;
class UInputComponent;
class UCharacterMovementComponent;

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

	/** Camera accessors */
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	USpringArmComponent* GetSpringArm() const { return SpringArm; }

public:
	void MoveToLocation(const FVector& InTarget);
	void StopAutoMove();

	void MoveByDirection(const FVector2D& Dir);
	void StopDirectionalMove();

	void MoveTowardMouse(const FVector2D& MousePos);
	void StopHoldMove();

private:
	// Auto move
	FVector TargetLocation;
	FTimerHandle AutoMoveHandle;
	void AutoMoveStep();

	// Hold move
	FVector HoldDirection;
	FTimerHandle HoldMoveHandle;
	void HoldMoveStep();

protected:
	//---------------------------------------
	// Components
	//---------------------------------------

	/** Spring arm for camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	/** 캐릭터 전용 MovementComponent (캐스팅 없이 사용) */
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> LMCharacterMovement;


protected:
	//---------------------------------------
	// Camera settings (편의 변수)
	//---------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings", meta = (ClampMin = "200.0", ClampMax = "3000.0"))
	float DefaultArmLength = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings", meta = (ClampMin = "-85.0", ClampMax = "-10.0"))
	float DefaultArmPitch = -55.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	bool bEnableCameraLag = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings", meta = (ClampMin = "0.0"))
	float CameraLagSpeed = 10.f;


protected:
	//---------------------------------------
	// Helpers
	//---------------------------------------

	/** 초기 컴포넌트 세팅 */
	void InitializeComponents();

	/** Movement component 안전하게 가져오기 */
	void CacheMovementComponent();

	/** 모바일 전용 카메라 보정 (optional) */
	void UpdateCameraForMobile(float DeltaTime);

private:
	/** 내부 상태 */
	UPROPERTY()
	FVector2D PendingMoveInput;
};
