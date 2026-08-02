// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthComponent.h"
#include "SimpleRPG/AbilitySystem/SimpleRPGAttributeSet.h"
#include "SimpleRPG/Character/SimpleRPGPlayerCharacter.h"
#include "SimpleRPG/Player/SimpleRPGPlayerState.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


// Sets default values for this component's properties
UPlayerHealthComponent::UPlayerHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPlayerHealthComponent::BindToAttributeSet()
{
	ASimpleRPGPlayerCharacter* OwnerPawn = Cast<ASimpleRPGPlayerCharacter>(GetOwner());
	if (!IsValid(OwnerPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	ASimpleRPGPlayerState* PS = Cast<ASimpleRPGPlayerState>(OwnerPawn->GetPlayerState());
	if (!IsValid(PS))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	USimpleRPGAttributeSet* AS = Cast<USimpleRPGAttributeSet>(PS->GetAttributeSet());
	if (!IsValid(AS))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player AttributeSet Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	AS->OnPlayerDamaged.AddDynamic(this, &UPlayerHealthComponent::HandleDamaged);
	AS->OnPlayerHPDepleted.AddDynamic(this, &UPlayerHealthComponent::HandleDeath);
}

void UPlayerHealthComponent::HandleDamaged(AActor* Instigator, FGameplayTagContainer AssetTags)
{
	AActor* OwnerPawn = GetOwner();
	if (!IsValid(OwnerPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	FGameplayTag EventTag;
	FGameplayEventData PayLoad;

	if (AssetTags.HasTag(SimpleRPGGameplayTags::HitReact_Stagger))
	{
		EventTag = SimpleRPGGameplayTags::Event_Combat_Stagger;
	}
	else if (AssetTags.HasTag(SimpleRPGGameplayTags::HitReact_KnockDown))
	{
		EventTag = SimpleRPGGameplayTags::Event_Combat_KnockDown;
	}
	else
	{
		//TODO : 추후 HitReat None 확인시 삭제 요망.
		UE_LOG(LogTemp, Warning, TEXT("None HitReact Tag"));
	}

	if (EventTag.IsValid())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerPawn, EventTag, PayLoad);
	}
	
}

void UPlayerHealthComponent::HandleDeath(AActor* Instigator)
{
	AActor* OwnerPawn = GetOwner();
	if (!IsValid(OwnerPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn Invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	FGameplayEventData PayLoad;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerPawn, SimpleRPGGameplayTags::Event_Combat_Death, PayLoad);
}



