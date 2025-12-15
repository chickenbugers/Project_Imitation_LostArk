// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerController/LM_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/Player/LM_Character_Player.h"

ALM_PlayerController::ALM_PlayerController()
{
	bShowMouseCursor = true;
}

void ALM_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(IMC_PCInput, 0);
    }
}

void ALM_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EI = CastChecked<UEnhancedInputComponent>(InputComponent);

    EI->BindAction(IA_Click, ETriggerEvent::Started, this, &ALM_PlayerController::OnClickTriggered);
    EI->BindAction(IA_Click, ETriggerEvent::Completed, this, &ALM_PlayerController::OnClickTriggered);

    EI->BindAction(IA_Hold, ETriggerEvent::Triggered, this, &ALM_PlayerController::OnHoldTriggered);
    EI->BindAction(IA_Hold, ETriggerEvent::Completed, this, &ALM_PlayerController::OnHoldTriggered);

    EI->BindAction(IA_MousePosition, ETriggerEvent::Triggered, this, &ALM_PlayerController::OnMousePosition);
}

void ALM_PlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (bHolding)
    {
        ALM_Character_Player* playercharacter = Cast<ALM_Character_Player>(GetPawn());
        if (playercharacter)
        {
            playercharacter->MoveTowardMouse(CachedMousePos);
        }
    }
}

void ALM_PlayerController::RequestMoveToLocation(const FVector& Dest)
{
    // 리슨 서버에서 Host는 이미 Server
    if (HasAuthority())
    {
        Server_MoveToLocation(Dest);
    }
    else
    {
        Server_MoveToLocation(Dest);
    }
}

void ALM_PlayerController::Server_MoveToLocation_Implementation(const FVector& Dest)
{
    APawn* pawn = GetPawn();
    if (!pawn) return;

    ALM_Character_Player* character = Cast<ALM_Character_Player>(pawn);
    if (!character) return;

    character->Server_MoveToLocation(Dest);
}

void ALM_PlayerController::OnMousePosition(const FInputActionValue& Value)
{
    CachedMousePos = Value.Get<FVector2D>();
}

void ALM_PlayerController::OnClickTriggered(const FInputActionValue& Value)
{
    bool bIsPressed = Value.Get<bool>();

    if (bIsPressed)
    {
        // 마우스 버튼 누름
        bPressed = true;
        PressTime = GetWorld()->GetTimeSeconds();

        // 바로 이동 
        MoveToClickLocation();
    }
    else
    {
        // 마우스 버튼 뗌
        float HeldTime = GetWorld()->GetTimeSeconds() - PressTime;
        bPressed = false;

        // 홀딩 종료
        if (bHolding)
        {
            bHolding = false;
            ALM_Character_Player* playercharacter = Cast<ALM_Character_Player>(GetPawn());
            if (playercharacter)
                playercharacter->StopHoldMove();
        }
    }
}

void ALM_PlayerController::OnHoldTriggered(const FInputActionValue& Value)
{
    if (!bPressed)
        return;

    bHolding = true;
}

void ALM_PlayerController::MoveToClickLocation()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (!Hit.bBlockingHit)
        return;

    RequestMoveToLocation(Hit.Location);
}