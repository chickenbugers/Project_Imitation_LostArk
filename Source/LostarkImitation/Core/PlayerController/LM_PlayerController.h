// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "LM_PlayerController.generated.h"

/**
 * 
 */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

UCLASS()
class LOSTARKIMITATION_API ALM_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALM_PlayerController();

public:
    virtual void SetupInputComponent() override;

protected:
    /** Time Threshold to know if it was a short press */
    UPROPERTY(EditAnywhere, Category = "Input")
    float ShortPressThreshold;
    // Input Mapping Context
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> IMC_PCInput;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Click;

    /** FX Class that we will spawn when clicking */
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UNiagaraSystem> FXCursor;

    // 터치 상태
    float PressTime = 0.f;
    /** True if the controlled character should navigate to the mouse cursor. */
    uint32 bMoveToMouseCursor : 1;
    /** Set to true if we're using touch input */
    uint32 bIsTouch : 1;
	// 위치 저장
    FVector CachedDestination;

protected:
    // Handler
    void OnInputStarted();
    void OnSetDestinationTriggered();
    void OnSetDestinationReleased();;
};
