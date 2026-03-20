// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_SendGameplayEvent.generated.h"

/**
 * 
 */

/* Montage Editor에서 이 NotifyState를 배치 할 때에 Inline으로 Property를 편집할 수 있게 해줌 */
UCLASS(EditInlineNew)
class SIMPLERPG_API UANS_SendGameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	/* EidtAnywhere : Montage에 배치한 뒤 인스턴스마다 다른 Tag를 설정할 수 있게 해줌*/

	UPROPERTY(EditAnywhere, Category = "GameplayEvent")
	FGameplayTag BeginTag;

	UPROPERTY(EditAnywhere, Category = "GameplayEvent")
	FGameplayTag EndTag;

public: 

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
	) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
};
