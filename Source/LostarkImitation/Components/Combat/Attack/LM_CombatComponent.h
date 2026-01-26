// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Types/LM_SkillType.h"
#include "LM_CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTARKIMITATION_API ULM_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    void RequestAttack(int32 SkillID);

    UFUNCTION(Server, Reliable)
    void Server_Attack(E_AttackType AttackType);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayMontage(UAnimMontage* Montage);
};
