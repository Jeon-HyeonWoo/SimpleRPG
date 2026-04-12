// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_DashMove.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UAT_DashMove::Activate()
{
	Super::Activate();

	/* Tick 활성화 */
	bTickingTask = true;

	/* Owner GameAbility에서 ActorInfo내의 AvatarActor */
	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());

	if (!Character)
	{
		OnCompleted.Broadcast();
		EndTask();
		return;
	}

	UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
	if (!MovementComp)
	{
		OnCompleted.Broadcast();
		EndTask(); 
		return;
	}

	//회전 설정 백업 후 비활성
	bOriginalOrientRotationToMovement = MovementComp->bOrientRotationToMovement;
	MovementComp->bOrientRotationToMovement = false;

	if (!DashDirection.IsNearlyZero())
	{
		FRotator DashRotation = DashDirection.Rotation();
		Character->SetActorRotation(FRotator(0.0f, DashRotation.Yaw, 0.0f));
	}
}

void UAT_DashMove::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	//경과 시간 누적
	ElapsedTime += DeltaTime;

	if (ElapsedTime >= DashDuration)
	{
		OnCompleted.Broadcast();
		EndTask();
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!Character)
	{
		OnCompleted.Broadcast();
		EndTask();
		return;
	}

	UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
	if (!MovementComp)
	{
		OnCompleted.Broadcast();
		EndTask();
		return;
	}

	//매 프레임 Velocity 세팅
	FVector DashVelocity = DashDirection * DashSpeed;
	MovementComp->Velocity = FVector(DashVelocity.X, DashVelocity.Y, 0.0f);
} 

void UAT_DashMove::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (Character)
	{
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();

		if (MovementComp)
		{
			//회전 설정 복원
			MovementComp->bOrientRotationToMovement = bOriginalOrientRotationToMovement;

			//잔여 속도 제거
			MovementComp->Velocity = FVector::ZeroVector;
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

UAT_DashMove* UAT_DashMove::CreateDashMove(UGameplayAbility* OwningAbility, FVector Direction, float Distance, float Duration)
{
	/*
	* NewAbilityTask : Task의 소유 GameAbility와 자동으로 연결해주는 함수,
	* GA가 Destory되면 Task도 같이 Destroy
	*/
	UAT_DashMove* Task = NewAbilityTask<UAT_DashMove>(OwningAbility);

	Task->DashDirection = Direction.GetSafeNormal();
	Task->DashSpeed = Distance / Duration;
	Task->DashDuration = Duration;
	Task->ElapsedTime = 0.0f;

	return Task;
}
