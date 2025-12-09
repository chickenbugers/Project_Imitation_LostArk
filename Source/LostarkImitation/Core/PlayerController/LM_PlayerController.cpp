// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerController/LM_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/Player/LM_Character_Player.h"

ALM_PlayerController::ALM_PlayerController()
{

}

void ALM_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(IMC_DefaultTouch, 0);
    }
}

void ALM_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);

    EIC->BindAction(IA_Touch, ETriggerEvent::Triggered, this, &ALM_PlayerController::OnTouchTriggered);
    EIC->BindAction(IA_TouchHold, ETriggerEvent::Triggered, this, &ALM_PlayerController::OnTouchHold);
    EIC->BindAction(IA_TouchRelease, ETriggerEvent::Triggered, this, &ALM_PlayerController::OnTouchReleased);
}

void ALM_PlayerController::OnTouchTriggered(const FInputActionValue& Value)
{
    FVector2D ScreenPos = Value.Get<FVector2D>();

    TouchStartPos = ScreenPos;
    TouchStartTime = GetWorld()->GetTimeSeconds();
    bIsHolding = false;
    HoldDelta = FVector2D::ZeroVector;
}

void ALM_PlayerController::OnTouchHold(const FInputActionValue& Value)
{
    FVector2D ScreenPos = Value.Get<FVector2D>();

    float Elapsed = GetWorld()->GetTimeSeconds() - TouchStartTime;

    // 짧은 탭 → 홀드 전환
    if (!bIsHolding && Elapsed > ShortTouchThreshold)
    {
        bIsHolding = true;

        // 자동 이동 중단
        if (ALM_Character_Player* PC = Cast<ALM_Character_Player>(GetPawn()))
        {
            PC->StopAutoMove();
        }
    }

    if (bIsHolding)
    {
        HoldDelta = (ScreenPos - TouchStartPos).GetSafeNormal();

        if (ALM_Character_Player* PC = Cast<ALM_Character_Player>(GetPawn()))
        {
            PC->MoveByDirection(HoldDelta);
        }
    }
}

void ALM_PlayerController::OnTouchReleased(const FInputActionValue& Value)
{
    FVector2D ScreenPos = Value.Get<FVector2D>();

    float Duration = GetWorld()->GetTimeSeconds() - TouchStartTime;

    // -----------------------
    // 1) 짧은 터치 → 목적지 이동
    // -----------------------
    if (!bIsHolding && Duration < ShortTouchThreshold)
    {
        FVector HitLoc;
        if (DeprojectToGround(ScreenPos, HitLoc))
        {
            if (auto* PC = Cast<ALM_Character_Player>(GetPawn()))
            {
                PC->MoveToLocation(HitLoc);
            }
        }
    }

    // -----------------------
    // 2) 홀드 이동 중 → 중단
    // -----------------------
    if (bIsHolding)
    {
        if (auto* PC = Cast<ALM_Character_Player>(GetPawn()))
        {
            PC->StopDirectionalMove();
        }
    }

    bIsHolding = false;
    HoldDelta = FVector2D::ZeroVector;
}

bool ALM_PlayerController::DeprojectToGround(const FVector2D& Screen, FVector& OutHit)
{
    FVector Origin, Direction;
    if (!DeprojectScreenPositionToWorld(Screen.X, Screen.Y, Origin, Direction))
        return false;

    FHitResult Hit;
    FVector End = Origin + Direction * 20000.f;

    if (GetWorld()->LineTraceSingleByChannel(Hit, Origin, End, ECC_Visibility))
    {
        OutHit = Hit.ImpactPoint;
        return true;
    }

    return false;
}