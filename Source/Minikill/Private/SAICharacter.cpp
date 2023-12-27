// Fill out your copyright notice in the Description page of Project Settings.


#include "SAICharacter.h"
#include "MAttributeComponent.h"
#include "Revolver.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");

	AttributeComponent = CreateDefaultSubobject<UMAttributeComponent>(TEXT("Attribute Component"));
}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();

	const FTransform handSocket = GetMesh()->GetSocketTransform(TEXT("Hand_rSocket"));

	// Spawn Revolver
	AActor* revolver = GetWorld()->SpawnActor(RevolverBP, &handSocket);
	revolver->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Hand_rSocket"));
	revolver->SetActorRelativeLocation(FVector(4.044628f, -17.122265f, 4.706299f));
	revolver->SetActorRelativeRotation(FRotator(74.198952f, 118.376793f, -71.397888f));
	Revolver = Cast<ARevolver>(revolver);
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensing->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent* bbc = AIC->GetBlackboardComponent();
		bbc->SetValueAsObject("TargetActor", Pawn);
	}
}


