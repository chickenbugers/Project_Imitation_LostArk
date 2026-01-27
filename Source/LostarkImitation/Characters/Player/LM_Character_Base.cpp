// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/LM_Character_Base.h"
#include "Components/Combat/Attack/LM_CombatComponent.h"

// Sets default values
ALM_Character_Base::ALM_Character_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	CombatComponent = CreateDefaultSubobject<ULM_CombatComponent>(TEXT("CombatComponent"));
}

void ALM_Character_Base::RequestAttack(int32 SkillID)
{
	if (!CombatComponent)
		return;

	CombatComponent->RequestAttack(0);
}

// Called when the game starts or when spawned
void ALM_Character_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALM_Character_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALM_Character_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

