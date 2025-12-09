// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "LM_PlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;

UCLASS()
class LOSTARKIMITATION_API ALM_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALM_PlayerController();

public:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    // Enhanced Input
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* IMC_DefaultTouch;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Touch;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_TouchHold;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_TouchRelease;

private:
    // 터치 상태
    float TouchStartTime = 0.f;
    FVector2D TouchStartPos;
    bool bIsHolding = false;
    FVector2D HoldDelta;

    float ShortTouchThreshold = 0.15f;

    // Handler
    void OnTouchTriggered(const FInputActionValue& Value);
    void OnTouchHold(const FInputActionValue& Value);
    void OnTouchReleased(const FInputActionValue& Value);

    bool DeprojectToGround(const FVector2D& Screen, FVector& OutHit);
};
