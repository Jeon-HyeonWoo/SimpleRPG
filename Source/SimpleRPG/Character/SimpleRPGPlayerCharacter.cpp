// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleRPGPlayerCharacter.h"

//Unreal header
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
//SimpleRPG header
#include "../Player/SimpleRPGPlayerState.h"
#include "../GameMode/SimpleRPGGameMode.h"
#include "PawnData.h"
#include "../AbilitySystem/SimpleRPGAbilitySet.h"
#include "../Input/SimpleRPGInputConfig.h"
#include "../Input/SimpleRPGInputComponent.h"







ASimpleRPGPlayerCharacter::ASimpleRPGPlayerCharacter()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	//Create SpringArmComponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARMCOMPONENT"));
	if (IsValid(SpringArmComponent))
	{
		SpringArmComponent->SetupAttachment(RootComponent);
		SpringArmComponent->TargetArmLength = 400.0f;
		SpringArmComponent->SocketOffset = FVector(0, 50, 70);
		SpringArmComponent->bUsePawnControlRotation = true;

		SpringArmComponent->bEnableCameraLag = true;
		SpringArmComponent->CameraLagSpeed = 10.0f;
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, SpringArmComponent is invalid"), __LINE__, __FUNCTION__);
	}

	//Create CameraComponent
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
	* 1. Connecting CharacterBase's ASC to PlayerState's ASC
	* 2. GiveToAbilitySystem from GameMode's PawnData
	*/
	ASimpleRPGPlayerState* SimpleRPGPlayerState = Cast<ASimpleRPGPlayerState>(GetPlayerState());

	if (IsValid(SimpleRPGPlayerState))
	{
		AbilitySystemComponent = SimpleRPGPlayerState->GetAbilitySystemComponent();

		if (HasAuthority())
		{
			ASimpleRPGGameMode* SimpleRPGGameMode = Cast<ASimpleRPGGameMode>(GetWorld()->GetAuthGameMode());
			if (IsValid(SimpleRPGGameMode))
			{
				const UPawnData* PawnData = SimpleRPGGameMode->DefaultPawnData;
				if (IsValid(PawnData) && IsValid(PawnData->AbilitySet))
				{
					PawnData->AbilitySet->GiveToAbilitySystem(AbilitySystemComponent);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("%d, %hs, PawnData or AbilitySet is invalid"), __LINE__, __FUNCTION__);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%d, %hs, GameMode is invalid"), __LINE__, __FUNCTION__);
			}
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState is invalid"));
	}

	/*
	* LinkAnimClassLayer Test code
	*/
	if (USkeletalMeshComponent* MeshComponent = GetMesh())
	{
		UE_LOG(LogTemp, Warning, TEXT("UnArmedAnimLayer: %s"),
			UnArmedAnimLayer ? *UnArmedAnimLayer->GetName() : TEXT("nullptr"));
		MeshComponent->LinkAnimClassLayers(UnArmedAnimLayer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MeshComponent is invalid"));
	}
	
	/*
	* 
	*/
	

}

void ASimpleRPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ASimpleRPGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	USimpleRPGInputComponent* SRPGIC = CastChecked<USimpleRPGInputComponent>(PlayerInputComponent);

	//Native Bind
	SRPGIC->BindNativeAction(InputConfig, FGameplayTag::RequestGameplayTag(FName("Input.Move")),
		ETriggerEvent::Triggered, this, &ThisClass::MoveHandler);

	SRPGIC->BindNativeAction(InputConfig, FGameplayTag::RequestGameplayTag(FName("Input.Look")),
		ETriggerEvent::Triggered, this, &ThisClass::LookHandler);

	//Ability Bind Pressed and Released
	SRPGIC->BindAbilityActions(InputConfig, this, &ThisClass::OnAbilityInputPressed, &ThisClass::OnAbilityInputReleased);
}

void ASimpleRPGPlayerCharacter::MoveHandler(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	if (MovementVector.X != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, MovementVector.X);
	}

	if (MovementVector.Y != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, MovementVector.Y);
	}
}

void ASimpleRPGPlayerCharacter::LookHandler(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (LookVector.X != 0.0f)
	{
		AddControllerYawInput(LookVector.X);
	}
	if (LookVector.Y != 0.0f)
	{
		AddControllerPitchInput(-LookVector.Y);
	}
}

void ASimpleRPGPlayerCharacter::OnAbilityInputPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	//ASC nullptr check
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, AbilitySystemComponent is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	//cast tag to id
	const int32 InputID = static_cast<int32>(InputConfig->FindAbilityInputIDByTag(InputTag));
	if (InputID != static_cast<int32>(ESimpleRPGAbilityInputID::None))
	{
		AbilitySystemComponent->AbilityLocalInputPressed(InputID);
	}
}

void ASimpleRPGPlayerCharacter::OnAbilityInputReleased(const FInputActionValue& Value, FGameplayTag InputTag)
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, AbilitySystemComponent is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	//cast tag to id
	const int32 InputID = static_cast<int32>(InputConfig->FindAbilityInputIDByTag(InputTag));

	if (InputID != static_cast<int32>(ESimpleRPGAbilityInputID::None))
	{
		AbilitySystemComponent->AbilityLocalInputReleased(InputID);
	}
}



