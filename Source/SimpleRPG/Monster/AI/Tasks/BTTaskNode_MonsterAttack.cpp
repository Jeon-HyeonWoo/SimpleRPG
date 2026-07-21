// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MonsterAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
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

	UAbilitySystemComponent* ASC = GetMonsterASC(&OwnerComp);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Failed;
	}

	//Instancing OwnerComp and AbilityEnd DelegateBind
	FBTMonsterAttackMemory* Memory = CastInstanceNodeMemory<FBTMonsterAttackMemory>(NodeMemory);
	Memory->OwnerComp = &OwnerComp;
	Memory->DelegateHandle = ASC->OnAbilityEnded.AddUObject(this, &UBTTaskNode_MonsterAttack::OnAbilityEnded, NodeMemory);

	//Send ActionTag
	FGameplayEventData PayLoad;
	PayLoad.InstigatorTags.AddTag(ActionTag);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MonsterPawn, EventTag, PayLoad);


	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTaskNode_MonsterAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTMonsterAttackMemory* Memory = CastInstanceNodeMemory<FBTMonsterAttackMemory>(NodeMemory);
	if (!Memory)
	{
		UE_LOG(LogTemp, Error, TEXT("Memory Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Aborted;
	}

	UAbilitySystemComponent* ASC = GetMonsterASC(&OwnerComp);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return EBTNodeResult::Aborted;
	}

	ASC->OnAbilityEnded.Remove(Memory->DelegateHandle);
	Memory->DelegateHandle.Reset();

	return EBTNodeResult::Aborted;
}

uint16 UBTTaskNode_MonsterAttack::GetInstanceMemorySize() const
{
	return Super::GetInstanceMemorySize() + sizeof(*this);
}

void UBTTaskNode_MonsterAttack::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData, uint8* NodeMemory)
{
	FBTMonsterAttackMemory* Memory = CastInstanceNodeMemory<FBTMonsterAttackMemory>(NodeMemory);
	if (!Memory)
	{
		UE_LOG(LogTemp, Error, TEXT("Memory Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	UBehaviorTreeComponent* BTComp = Memory->OwnerComp.Get();
	if (!BTComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BTComponent Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	UAbilitySystemComponent* ASC = GetMonsterASC(BTComp);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	ASC->OnAbilityEnded.Remove(Memory->DelegateHandle);
	Memory->DelegateHandle.Reset();

	EBTNodeResult::Type Result = AbilityEndedData.bWasCancelled ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
	FinishLatentTask(*BTComp, Result);
}

UAbilitySystemComponent* UBTTaskNode_MonsterAttack::GetMonsterASC(UBehaviorTreeComponent* InOwnerComp) const
{
	if (!InOwnerComp)
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerComponent Invalid"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	AAIController* AIController = InOwnerComp->GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("AIControlelr Invalid"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	ASimpleRPGMonsterBase* MonsterPawn = Cast<ASimpleRPGMonsterBase>(AIController->GetPawn());
	if (!MonsterPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterPawn Invalid"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	UAbilitySystemComponent* ASC = MonsterPawn->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC Invalid"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	return ASC;
}

