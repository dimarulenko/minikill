// Fill out your copyright notice in the Description page of Project Settings.


#include "Revolver.h"
#include "GameplayTagsModule.h"
#include "Components/ArrowComponent.h"

// Sets default values
ARevolver::ARevolver()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Muzzle = CreateDefaultSubobject<UArrowComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(Mesh);

	ActionComponent = CreateDefaultSubobject<UMActionComponent>(TEXT("Action Component"));
	AttributeComponent = CreateDefaultSubobject<UMAttributeComponent>(TEXT("Attribute Component"));
}

// Called when the game starts or when spawned
void ARevolver::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ARevolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

