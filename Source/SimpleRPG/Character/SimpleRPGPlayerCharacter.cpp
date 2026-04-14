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
#include "../AbilitySystem/SimpleRPGAttributeSet.h"
#include "../Equipment/SimpleRPGEquipmentComponent.h"
#include "../Weapon/WeaponData.h"







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

	//Create EquipmentComponent
	EquipmentComponent = CreateDefaultSubobject<USimpleRPGEquipmentComponent>(TEXT("EQUIPMENTCOMPONENT"));
	if (!IsValid(EquipmentComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, EquipmentComponent is invalid"), __LINE__, __FUNCTION__);
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
		AbilitySystemComponent->InitAbilityActorInfo(SimpleRPGPlayerState, this);

		/* AttributeSet(Character Stat) Initialize */
		if (USimpleRPGAttributeSet* AS = const_cast<USimpleRPGAttributeSet*>(SimpleRPGPlayerState->GetAttributeSet()))
		{
			AS->InitMovementSpeed(600.0f);
			AS->InitHealth(100.0f); 
			AS->InitMaxHealth(100.0f);
		}

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
				
				if (IsValid(EquipmentComponent) && PawnData->WeaponSlots.Num() > 0)
				{
					EquipmentComponent->InitializeWeaponSlot(PawnData->WeaponSlots);
					EquipmentComponent->EquipWeapon(PawnData->WeaponSlots[0]);
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

	//Weapon Slot Bind
	SRPGIC->BindNativeAction(InputConfig, FGameplayTag::RequestGameplayTag(FName("Input.WeaponSwap1")),
		ETriggerEvent::Started, this, &ThisClass::WeaponslotHandler);
	SRPGIC->BindNativeAction(InputConfig, FGameplayTag::RequestGameplayTag(FName("Input.WeaponSwap2")),
		ETriggerEvent::Started, this, &ThisClass::WeaponslotHandler);

	//Ability Bind Pressed and Released
	SRPGIC->BindAbilityActions(InputConfig, this, &ThisClass::OnAbilityInputPressed, &ThisClass::OnAbilityInputReleased);
}

void ASimpleRPGPlayerCharacter::MoveHandler(const FInputActionValue& Value, FGameplayTag InputTag)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	FVector InputRightVector = MovementRotation.RotateVector(FVector::RightVector) * MovementVector.X;
	FVector InputForwardVector = MovementRotation.RotateVector(FVector::ForwardVector) * MovementVector.Y;

	LastInputDirection = (InputRightVector + InputForwardVector).GetSafeNormal();
	LastInputTime = GetWorld()->GetTimeSeconds();

	if (AbilitySystemComponent)
	{
		const USimpleRPGAttributeSet* AS = AbilitySystemComponent->GetSet<USimpleRPGAttributeSet>();
		if (AS && (AS->GetMovementSpeed() <= 0.0f))
		{
			return;
		}
	}

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

void ASimpleRPGPlayerCharacter::LookHandler(const FInputActionValue& Value, FGameplayTag InputTag)
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

void ASimpleRPGPlayerCharacter::WeaponslotHandler(const FInputActionValue& Value, FGameplayTag InputTag)
{
	static const FGameplayTag Slot1 = FGameplayTag::RequestGameplayTag(FName("Input.WeaponSwap1"));
	static const FGameplayTag Slot2 = FGameplayTag::RequestGameplayTag(FName("Input.WeaponSwap2"));

	int SlotIndex = -1;

	if (InputTag == Slot1) SlotIndex = 0;
	else if (InputTag == Slot2) SlotIndex = 1;

	if (SlotIndex >= 0 && IsValid(EquipmentComponent))
	{
		EquipmentComponent->RequestWeaponSwap(SlotIndex);
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



