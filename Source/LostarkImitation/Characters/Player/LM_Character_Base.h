// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LM_Character_Base.generated.h"

class ULM_CombatComponent;

UCLASS()
class LOSTARKIMITATION_API ALM_Character_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALM_Character_Base();

public:
	void RequestAttack(int32 SkillID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	TObjectPtr<ULM_CombatComponent> CombatComponent;
};
