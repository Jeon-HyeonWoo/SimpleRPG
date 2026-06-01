// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_SwordHitTrace.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UANS_SwordHitTrace : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
	) override;

	virtual void NotifyTick(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference
	) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;

public:

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "SimpleRPG|HitTrace")
	float TraceDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|HitTrace")
	float TraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|HitTrace")
	float TraceStartZOffset;

	UPROPERTY()
	TSet<AActor*> HitActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|HitTag")
	FGameplayTag HitTag;

};
