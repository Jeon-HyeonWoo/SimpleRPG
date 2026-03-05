// Fill out your copyright notice in the Description page of Project Settings.

//Unreal header
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
//SimpleRPG header
#include "SimpleRPGPlayerCharacter.h"
#include "../Player/SimpleRPGPlayerState.h"






ASimpleRPGPlayerCharacter::ASimpleRPGPlayerCharacter()
{
	//Create SpringArmComponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARMCOMPONENT"));
	if (IsValid(SpringArmComponent))
	{
		SpringArmComponent->SetupAttachment(RootComponent);
		SpringArmComponent->TargetArmLength = 400.0f;
		SpringArmComponent->SocketOffset = FVector(0, 50, 70);
		SpringArmComponent->bUsePawnControlRotation = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, SpringArmComponent is invalid"), __LINE__, __FUNCTION__);
	}

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERACOMPONENT"));
	if (IsValid(CameraComponent))
	{
		CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
		CameraComponent->bUsePawnControlRotation = false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, CameraComponent is invalid"), __LINE__, __FUNCTION__);
	}
	
}

void ASimpleRPGPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	/*
	* Connecting CharacterBase's ASC to PlayerState's ASC
	*/
	ASimpleRPGPlayerState* SimpleRPGPlayerState = Cast<ASimpleRPGPlayerState>(GetPlayerState());

	if (IsValid(SimpleRPGPlayerState))
	{
		AbilitySystemComponent = SimpleRPGPlayerState->GetAbilitySystemComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState is invalid"));
	}

	
}
