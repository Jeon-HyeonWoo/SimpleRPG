// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UANS_SendGameplayEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	//Owner Mesh와 Tag 유효성 검사
	if (MeshComp && BeginTag.IsValid())
	{
		//Montage를 사용하는 Owner를 가져옴
		if (AActor* Owner = MeshComp->GetOwner())
		{
			//히트 판정 같은 곳에 데미지 수치나 타겟 정보를 담아서 보낼 수 있는 구조체
			FGameplayEventData PayLoad;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, BeginTag, PayLoad);
		}
	}
}

void UANS_SendGameplayEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && EndTag.IsValid())
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			FGameplayEventData PayLoad;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EndTag, PayLoad);
		}
	}
}
