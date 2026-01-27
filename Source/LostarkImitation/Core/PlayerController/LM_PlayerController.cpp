// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerController/LM_PlayerController.h"
#include "Characters/Player/LM_Character_Player.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

#include "LostarkImitation.h"

ALM_PlayerController::ALM_PlayerController()
{
	bMoveToMouseCursor = false;

	// configure the controller
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ALM_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Only set up input on local player controllers
    if (IsLocalPlayerController())
    {
        // Add Input Mapping Context
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(IMC_PCInput, 0);
        }

        if (UEnhancedInputComponent* EI = CastChecked<UEnhancedInputComponent>(InputComponent))
        {
            EI->BindAction(IA_Click, ETriggerEvent::Started, this, &ALM_PlayerController::OnInputStarted);
            EI->BindAction(IA_Click, ETriggerEvent::Triggered, this, &ALM_PlayerController::OnSetDestinationTriggered);
            EI->BindAction(IA_Click, ETriggerEvent::Completed, this, &ALM_PlayerController::OnSetDestinationReleased);
            EI->BindAction(IA_Click, ETriggerEvent::Canceled, this, &ALM_PlayerController::OnSetDestinationReleased);

			EI->BindAction(IA_Attack, ETriggerEvent::Started, this, &ALM_PlayerController::OnAttackPressed);
        }
        else
        {
            UE_LOG(LogLostarkImitation, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
        }
    }
}

void ALM_PlayerController::OnInputStarted()
{
	StopMovement();
}

void ALM_PlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	
	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ALM_PlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(
			this,
			CachedDestination
		);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

void ALM_PlayerController::OnAttackPressed()
{
	ALM_Character_Base* character_base = Cast<ALM_Character_Base>(GetPawn());

	if (character_base)
	{
		character_base->RequestAttack(0);
	}
}
