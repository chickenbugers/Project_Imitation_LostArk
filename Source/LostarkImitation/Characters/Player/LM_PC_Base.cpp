// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/LM_PC_Base.h"

// Sets default values
ALM_PC_Base::ALM_PC_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALM_PC_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALM_PC_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALM_PC_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

