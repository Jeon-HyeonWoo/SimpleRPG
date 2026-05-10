// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SwordHitTrace.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"

void UANS_SwordHitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	HitActors.Empty();
}

void UANS_SwordHitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("OnwerActor not found, %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	FVector TraceStart = OwnerActor->GetActorLocation();
	FVector TraceEnd = TraceStart + (OwnerActor->GetActorForwardVector() * TraceDistance);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerActor);

	TArray<FHitResult> OutHits;

	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		OwnerActor->GetWorld(),
		TraceStart,
		TraceEnd,
		TraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		IgnoreActors,
		EDrawDebugTrace::ForOneFrame,
		OutHits,
		true
	);

	if (bHit)
	{
		for (const auto& HitActor : OutHits)
		{
			if ((HitActor.GetActor() != nullptr) && !HitActors.Contains(HitActor.GetActor()))
			{
				HitActors.Add(HitActor.GetActor());
				FGameplayEventData PayLoad;			
				PayLoad.Instigator = OwnerActor;		//Event 시작 주체
				PayLoad.Target = HitActor.GetActor();	//Event 판정 대상
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, HitTag, PayLoad);
				
			}
		}
	}
}

void UANS_SwordHitTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	HitActors.Empty();
}
