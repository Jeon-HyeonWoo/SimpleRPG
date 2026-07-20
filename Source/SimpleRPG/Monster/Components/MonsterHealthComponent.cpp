// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHealthComponent.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "SimpleRPG/UI/MonsterHPBarWidget.h"
#include "SimpleRPG/Monster/AbilitySystem/AttributeSet/SimpleRPGMonsterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Components/WidgetComponent.h"

UMonsterHealthComponent::UMonsterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMonsterHealthComponent::InitializeWithOwner(ASimpleRPGMonsterBase* InMonster)
{
	if (!ensureMsgf(IsValid(InMonster), TEXT("InMonster invalid"))) return;

	MonsterCharacter = InMonster;
	BindDelegate();
}

void UMonsterHealthComponent::HandleDamaged(AActor* _Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("%d, %hs"), __LINE__, __FUNCTION__);
	FGameplayEventData PayLoad;
	PayLoad.InstigatorTags.AddTag(SimpleRPGGameplayTags::Monster_Ability_Stagger);
	UE_LOG(LogTemp, Warning, TEXT("%d, %hs"), __LINE__, __FUNCTION__);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), SimpleRPGGameplayTags::Monster_Event_Stagger, PayLoad);
	
	UpdateHPBar();
	ReportDamageToPerception(_Instigator);
}

void UMonsterHealthComponent::HandleDeath(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("%d, %hs"), __LINE__, __FUNCTION__);
	if (bIsDead) return;

	bIsDead = true;

	UpdateHPBar();

	FGameplayEventData PayLoad;
	PayLoad.InstigatorTags.AddTag(SimpleRPGGameplayTags::Monster_Ability_Death);
	UE_LOG(LogTemp, Warning, TEXT("%d, %hs"), __LINE__, __FUNCTION__);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), SimpleRPGGameplayTags::Monster_Event_Death, PayLoad);

	
}

void UMonsterHealthComponent::RestoreHP()
{
	USimpleRPGMonsterAttributeSet* MonsterAS = GetMonsterAS();
	if (!MonsterAS) return;

	MonsterAS->SetHP(MonsterAS->GetMaxHP());
	UpdateHPBar();
}

void UMonsterHealthComponent::UpdateHPBar()
{
	USimpleRPGMonsterAttributeSet* MonsterAS = GetMonsterAS();
	if (!MonsterAS) return;

	ASimpleRPGMonsterBase* Monster = MonsterCharacter.Get();
	if (!Monster) return;

	UWidgetComponent* WComponent = Monster->HPBarWidgetComponent;
	if (!IsValid(WComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("Monster's(Name : %s) WidgetComponent invalid : %d, %hs"), *MonsterCharacter->GetName(), __LINE__, __FUNCTION__);
		return;
	}

	UMonsterHPBarWidget* WMonsterHPBar = Cast<UMonsterHPBarWidget>(MonsterCharacter->HPBarWidgetComponent->GetWidget());
	if (!WMonsterHPBar)
	{
		UE_LOG(LogTemp, Error, TEXT("Monster's(Name : %s) Widget invalid : %d, %hs"), *MonsterCharacter->GetName(), __LINE__, __FUNCTION__);
		return;
	}
	
	WMonsterHPBar->SetHP(MonsterAS->GetHP(), MonsterAS->GetMaxHP());
}

void UMonsterHealthComponent::BindDelegate()
{
	USimpleRPGMonsterAttributeSet* MonsterAS = GetMonsterAS();
	if (!MonsterAS) return;

	MonsterAS->OnHPDepleted.AddDynamic(this, &UMonsterHealthComponent::HandleDeath);
}

void UMonsterHealthComponent::ReportDamageToPerception(AActor* _Instigator)
{
	if (!_Instigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("Instigator is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		GetOwner(),			//DamagedActor(Monster)
		_Instigator,					//InstigatorActor(Player)
		1.0f,
		_Instigator->GetActorLocation(),
		GetOwner()->GetActorLocation()
		//Tag : Default Name_None, 필요하면 추가
	);
	
	
}

USimpleRPGMonsterAttributeSet* UMonsterHealthComponent::GetMonsterAS() const
{
	
	if (!MonsterCharacter.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Monster(Name : %s) invalid : %d, %hs"), *GetNameSafe(GetOwner()), __LINE__, __FUNCTION__);
		return nullptr;
	}

	USimpleRPGMonsterAttributeSet* MonsterAS = MonsterCharacter->GetAttributeSet();
	if (!ensureMsgf(IsValid(MonsterAS), TEXT("Monster AttributeSet : Invalid")))
	{
		return nullptr;
	}
	
	return MonsterAS;
}


