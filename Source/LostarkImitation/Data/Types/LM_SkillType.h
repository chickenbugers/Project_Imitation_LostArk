// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LM_SkillType.generated.h"

UENUM(BlueprintType)
enum class E_AttackType : uint8
{
    None            UMETA(DisplayName = "None"),
    Basic           UMETA(DisplayName = "Basic Attack"),
    Combo           UMETA(DisplayName = "Combo Attack"),
    Skill_Melee     UMETA(DisplayName = "Melee Skill"),
    Skill_Ranged    UMETA(DisplayName = "Ranged Skill"),
    Skill_Area      UMETA(DisplayName = "Area Skill"),
    Skill_Buff      UMETA(DisplayName = "Buff Skill"),
    Charge          UMETA(DisplayName = "Charge Attack"),
    Hold            UMETA(DisplayName = "Hold Attack"),
    Counter         UMETA(DisplayName = "Counter"),
    Finisher        UMETA(DisplayName = "Finisher"),
};