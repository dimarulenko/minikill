// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinikillCharacter.h"
#include "MinikillProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MActionComponent.h"
#include "MAttributeComponent.h"
#include "GameplayTagsModule.h"
#include "Revolver.h"
#include "Sabre.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMinikillCharacter

AMinikillCharacter::AMinikillCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a Spring arm
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	SpringArmComp->bUsePawnControlRotation = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = false;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	ActionComponent = CreateDefaultSubobject<UMActionComponent>(TEXT("Action Component"));
	AttributeComponent = CreateDefaultSubobject<UMAttributeComponent>(TEXT("Attribute Component"));
}

void AMinikillCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	const FTransform handSocket = Mesh1P->GetSocketTransform(TEXT("Hand_rSocket"));

	// Spawn Revolver
	AActor* revolver = GetWorld()->SpawnActor(RevolverBP, &handSocket);
	revolver->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Hand_rSocket"));
	revolver->SetActorRelativeLocation(FVector(4.044628f, -17.122265f, 4.706299f));
	revolver->SetActorRelativeRotation(FRotator(74.198952f, 118.376793f, -71.397888f));
	revolver->SetActorHiddenInGame(true);
	Revolver = Cast<ARevolver>(revolver);

	// Spawn Sabre
	AActor* sabre = GetWorld()->SpawnActor(SabreBP, &handSocket);
	sabre->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Hand_rSocket"));
	sabre->SetActorRelativeLocation(FVector(2.569257f, -9.551620f, 2.646324f));
	sabre->SetActorRelativeRotation(FRotator(69.409327f, 103.466391f, -80.651431f));
	Sabre = Cast<ASabre>(sabre);

}

//////////////////////////////////////////////////////////////////////////// Input

void AMinikillCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Shooting / Slashing
		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &AMinikillCharacter::Primary);

		// Aiming / Blocking
		EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &AMinikillCharacter::Secondary);

		// Reloading
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AMinikillCharacter::Reload);

		// SwapWeapons
		EnhancedInputComponent->BindAction(SwapWeaponsAction, ETriggerEvent::Started, this, &AMinikillCharacter::SwapWeapons);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMinikillCharacter::Move);

		// Sprinting/Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AMinikillCharacter::StartDash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &AMinikillCharacter::EndSprint);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMinikillCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMinikillCharacter::EndCrouch);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMinikillCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


static FGameplayTag crouchTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Crouch");
static FGameplayTag dashTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Dash");
static FGameplayTag sprintTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Sprint");
static FGameplayTag reloadTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Reload");
static FGameplayTag blockTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Block");
static FGameplayTag swapWeaponsTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.SwapWeapons");


void AMinikillCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMinikillCharacter::StartDash()
{
	ActionComponent->StartAction(this, dashTag);
	ActionComponent->StartAction(this, sprintTag);
}

void AMinikillCharacter::EndSprint()
{
	ActionComponent->StopAction(this, sprintTag);
}

void AMinikillCharacter::StartCrouch()
{
	ActionComponent->StartAction(this, crouchTag);
}

void AMinikillCharacter::EndCrouch()
{
	ActionComponent->StopAction(this, crouchTag);
}

void AMinikillCharacter::Primary()
{
	static FGameplayTag slashTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Slash");
	static FGameplayTag fireTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Fire");
	switch (EquippedWeapon)
	{
	case EEquippedWeapon::Revolver:
		if (Revolver == nullptr) return;
		Revolver->ActionComponent->StartAction(this, fireTag);
		break;
	case EEquippedWeapon::Sabre:
		if (Sabre == nullptr) return;
		Sabre->ActionComponent->StartAction(this, slashTag);
		break;
	}
}

void AMinikillCharacter::Secondary()
{
	switch (EquippedWeapon)
	{
	case EEquippedWeapon::Revolver:
		if (Revolver == nullptr) return;
		// TODO Aim
		break;
	case EEquippedWeapon::Sabre:
		if (Sabre == nullptr) return;
		Sabre->ActionComponent->StartAction(this, blockTag);
		break;
	}
}

void AMinikillCharacter::Reload()
{
	if (EquippedWeapon != EEquippedWeapon::Revolver) return;
	if (Revolver == nullptr) return;
	Revolver->ActionComponent->StartAction(this, reloadTag);
}

void AMinikillCharacter::SwapWeapons()
{
	ActionComponent->StartAction(this, swapWeaponsTag);
}

void AMinikillCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMinikillCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AMinikillCharacter::GetHasRifle()
{
	return bHasRifle;
}