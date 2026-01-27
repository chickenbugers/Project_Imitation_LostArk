// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LM_GameInstance.generated.h"

/**
 * 
 */

struct FLM_SkillData;

UCLASS()
class LOSTARKIMITATION_API ULM_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    virtual void Init() override;

    const FLM_SkillData* GetSkillData(int32 SkillID) const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "DataTable")
    UDataTable* SkillDataTable;

private:
    TMap<int32, const FLM_SkillData*> SkillDataMap;
};
