// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/LM_SkillType.h"
#include "LM_SkillData.generated.h"

USTRUCT(BlueprintType)
struct FLM_SkillData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    E_AttackType AttackType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UAnimMontage* Montage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CoolTime;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float StaminaCost;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Range;
};
