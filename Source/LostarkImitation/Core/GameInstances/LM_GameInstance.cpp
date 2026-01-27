// Fill out your copyright notice in the Description page of Project Settings.


#include "LM_GameInstance.h"
#include "Data/Skill/LM_SkillData.h"

void ULM_GameInstance::Init()
{
    Super::Init();

    if (!SkillDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("SkillDataTable is null"));
        return;
    }

    static const FString ContextString(TEXT("SkillDataContext"));
    TArray<FLM_SkillData*> Rows;
    SkillDataTable->GetAllRows(ContextString, Rows);

    for (const FLM_SkillData* Row : Rows)
    {
        if (!Row) continue;

        SkillDataMap.Add(Row->SkillID, Row);
    }

}

const FLM_SkillData* ULM_GameInstance::GetSkillData(int32 SkillID) const
{
    if (const FLM_SkillData* const* Found = SkillDataMap.Find(SkillID))
    {
        return *Found;
    }

    return nullptr;
}
