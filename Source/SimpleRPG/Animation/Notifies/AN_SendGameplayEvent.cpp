// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && EventTag.IsValid())
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			FGameplayEventData PayLoad;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, PayLoad);
		}
	}
}
