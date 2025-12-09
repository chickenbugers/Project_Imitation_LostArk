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

	/** Attribute component (HP, MP, Stats 등) - 예시 */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//TObjectPtr<UAttributeComponent> AttributeComponent;

	/** Skill component (스킬 로직 캡슐화) - 예시 */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//TObjectPtr<USkillComponent> SkillComponent;

	/** Equipment component (장비) - 예시 */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//TObjectPtr<UEquipmentComponent> EquipmentComponent;

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
	// Input handling functions
	//---------------------------------------

	/** 이동 (X = forward/backward, Y = right/left) */
	void Input_MoveForward(float Value);
	void Input_MoveRight(float Value);

	/** 카메라 회전 (터치 드래그 / 마우스) - 모바일: Yaw 처리 주로 사용 */
	void Input_LookUp(float Value);
	void Input_Turn(float Value);

	/** 기본 공격 */
	void Input_BasicAttack();

	/** 스킬 사용 (슬롯 인덱스 기반 예시) */
	void Input_UseSkill1();
	void Input_UseSkill2();

	/** 회피 */
	void Input_Dodge();

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
