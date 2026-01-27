// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LM_CombatComponent.generated.h"

struct FLM_SkillData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTARKIMITATION_API ULM_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULM_CombatComponent();

public:
    void RequestAttack(int32 SkillID);

    UFUNCTION(Server, Reliable)
    void Server_Attack(int32 SkillID);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayMontage(UAnimMontage* Montage);

protected:
    const FLM_SkillData* GetSkillData(int32 SkillID) const;
};
