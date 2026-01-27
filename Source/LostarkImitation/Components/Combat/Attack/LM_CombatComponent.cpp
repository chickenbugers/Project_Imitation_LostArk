// Fill out your copyright notice in the Description page of Project Settings.


#include "LM_CombatComponent.h"
#include "Data/Skill/LM_SkillData.h"
#include "Characters/Player/LM_Character_Base.h"
#include "Core/GameInstances/LM_GameInstance.h"

ULM_CombatComponent::ULM_CombatComponent()
{
	SetIsReplicatedByDefault(true);
}

// 서버에 요청
void ULM_CombatComponent::RequestAttack(int32 SkillID)
{
	// 공격 가능 여부 체크
    //if (!CanAttack(SkillID)) return;

    APawn* Pawn = Cast<APawn>(GetOwner());
    if (!Pawn || !Pawn->IsLocallyControlled()) return;

    Server_Attack(SkillID);
}


void ULM_CombatComponent::Server_Attack_Implementation(int32 SkillID)
{
    const FLM_SkillData* Data = GetSkillData(SkillID);

    if (!Data)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkillData not found. SkillID: %d"), SkillID);
        return;
    }

    // 서버에서 최종 검증
    //if(!CanAttack_Server(SkillData)) return;

    Data->SkillID;
    Data->AttackType;

	Multicast_PlayMontage(Data->Montage);
}

void ULM_CombatComponent::Multicast_PlayMontage_Implementation(UAnimMontage* Montage)
{
    //애니메이션 호출
    ALM_Character_Base* Character = Cast<ALM_Character_Base>(GetOwner());
    if (!Character || !Montage) return;

    Character->PlayAnimMontage(Montage);
}

const FLM_SkillData* ULM_CombatComponent::GetSkillData(int32 SkillID) const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    ULM_GameInstance* GI = Cast<ULM_GameInstance>(World->GetGameInstance());

    if (!GI) return nullptr;

    return GI->GetSkillData(SkillID);
}