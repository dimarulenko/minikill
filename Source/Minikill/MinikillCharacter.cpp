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
#include "MActionComponent.h"
#include "MAttributeComponent.h"
#include "GameplayTagsModule.h"
#include "Revolver.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMinikillCharacter

AMinikillCharacter::AMinikillCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
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

	// Spawn Revolver
	const FTransform handSocket = Mesh1P->GetSocketTransform(TEXT("Hand_rSocket"));
	AActor* gun = GetWorld()->SpawnActor(RevolverBP, &handSocket);
	gun->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Hand_rSocket"));
	gun->SetActorRelativeLocation(FVector(4.044628f, -17.122265f, 4.706299f));
	gun->SetActorRelativeRotation(FRotator(74.198952f, 118.376793f, -71.397888f));
	Revolver = Cast<ARevolver>(gun);
	
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

		// Shooting
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMinikillCharacter::Fire);

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

void AMinikillCharacter::Fire()
{
	if (Revolver == nullptr) return;
	Revolver->Fire();
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