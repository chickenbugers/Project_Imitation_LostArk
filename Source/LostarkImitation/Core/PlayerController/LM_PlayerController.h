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
    virtual void PlayerTick(float DeltaTime) override;

public:
    void RequestMoveToLocation(const FVector& Dest);

protected:
    UFUNCTION(Server, Reliable)
    void Server_MoveToLocation(const FVector& Dest);

private:
    // Input Mapping Context
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* IMC_PCInput;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Click;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Hold;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_MousePosition;

private:
    // 터치 상태
    bool bPressed = false;
    bool bHolding = false;
    float PressTime = 0.f;

    float ClickThreshold = 0.15f;

    // Handler
    void OnClickTriggered(const FInputActionValue& Value);
    void OnHoldTriggered(const FInputActionValue& Value);
    void OnMousePosition(const FInputActionValue& Value);

    FVector2D CachedMousePos;

    void MoveToClickLocation();
};
