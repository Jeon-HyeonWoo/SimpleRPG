// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MonsterAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "SimpleRPG/Monster/AI/MonsterAIController.h"

UBTTaskNode_MonsterAttack::UBTTaskNode_MonsterAttack()
{
	NodeName = "Monster Attack";
}

EBTNodeResult::Type UBTTaskNode_MonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MonsterPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterPawn is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = MonsterPawn->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	FGameplayEventData PayLoad;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MonsterPawn, EventTag, PayLoad);

	ASC->OnAbilityEnded.AddUObject(this, &UBTTaskNode_MonsterAttack::OnAbilityEnded);

	CachedOwnerComp = &OwnerComp;
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTaskNode_MonsterAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MonsterPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterPawn is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Aborted;
	}

	UAbilitySystemComponent* ASC = MonsterPawn->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Aborted;
	}

	ASC->OnAbilityEnded.RemoveAll(this);

	return EBTNodeResult::Aborted;
}

void UBTTaskNode_MonsterAttack::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData)
{
	if (!CachedOwnerComp)
	{
		UE_LOG(LogTemp, Error, TEXT("CachedOwnerComp is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(CachedOwnerComp->GetAIOwner()->GetPawn());
	if (!MonsterPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterPawn is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	UAbilitySystemComponent* ASC = MonsterPawn->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	ASC->OnAbilityEnded.RemoveAll(this);

	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}
