// Fill out your copyright notice in the Description page of Project Settings.


#include "Revolver.h"
#include "GameplayTagsModule.h"

// Sets default values
ARevolver::ARevolver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionComponent = CreateDefaultSubobject<UMActionComponent>(TEXT("Action Component"));
	AttributeComponent = CreateDefaultSubobject<UMAttributeComponent>(TEXT("Attribute Component"));
}

// Called when the game starts or when spawned
void ARevolver::BeginPlay()
{
	Super::BeginPlay();
}

void ARevolver::Fire()
{
	FGameplayTag fireTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Fire");
	FGameplayTag ammoTag = UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Ammo"); 
	float ammo = AttributeComponent->GetAttribute(ammoTag); 
	UE_LOG(LogTemp, Warning, TEXT("%f"), ammo);
	if (AttributeComponent->GetAttribute(ammoTag) > 0 && ActionComponent->StartAction(this, fireTag))
	{
		// Fire Weapon
		AttributeComponent->ApplyChange(ammoTag, -1);

		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &ARevolver::ReadyToFire, FireDelay, false);
	}
}

void ARevolver::ReadyToFire()
{
	// Ready to fire again
	FGameplayTag tag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Fire");
	ActionComponent->StopAction(this, tag);
}


// Called every frame
void ARevolver::Tick(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, ActionComponent->ActiveGameplayTags.ToString());
	Super::Tick(DeltaTime);
}

