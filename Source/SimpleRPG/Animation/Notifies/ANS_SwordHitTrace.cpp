// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SwordHitTrace.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"


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

	FVector TraceStart = OwnerActor->GetActorLocation() + FVector(0, 0, TraceStartZOffset);
	FVector TraceEnd = TraceStart + (OwnerActor->GetActorForwardVector() * TraceDistance);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerActor);

	TArray<FHitResult> OutHits;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		OwnerActor->GetWorld(),
		TraceStart,
		TraceEnd,
		TraceRadius,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForOneFrame,
		OutHits,
		true
	);

	if (bHit)
	{
		for (const auto& HitResult : OutHits)
		{
			AActor* HitActor = HitResult.GetActor();

			if (HitActor == nullptr) continue;
			if (!Cast<APawn>(HitActor)) continue;
			APawn* OwnerPawn = Cast<APawn>(OwnerActor);
			if (OwnerActor)
			{
				if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OwnerPawn->GetController()))
				{
					if (TeamAgent->GetTeamAttitudeTowards(*HitActor) != ETeamAttitude::Hostile)
					{
						continue;   // 적대 아니면 스킵 (아군 몬스터 등)
					}
				}
			}

			if (HitActors.Contains(HitActor)) continue;
			
			HitActors.Add(HitActor);
			FGameplayEventData PayLoad;
			PayLoad.Instigator = OwnerActor;
			PayLoad.Target = HitActor;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, HitTag, PayLoad);

		}
	}
}

void UANS_SwordHitTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	HitActors.Empty();
}
