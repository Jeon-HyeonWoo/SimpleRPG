// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGCharacterBase.h"

// Sets default values
ASimpleRPGCharacterBase::ASimpleRPGCharacterBase(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASimpleRPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimpleRPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ASimpleRPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called to bind functionality to input


