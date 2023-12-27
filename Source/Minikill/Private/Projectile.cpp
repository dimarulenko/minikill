// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MAttributeComponent.h"
#include "GameplayTagsModule.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	CollisionSphere->InitSphereRadius(0.1f);

	RootComponent = CollisionSphere;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = 10000.0f;
	ProjectileMovement->MaxSpeed = 10000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
	LastPos = GetActorLocation();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

static FGameplayTag healthTag = UGameplayTagsManager::Get().RequestGameplayTag("Attribute.Health");
static FGameplayTag alterTag = UGameplayTagsManager::Get().RequestGameplayTag("Attribute.AlterKill");

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UMAttributeComponent* hitAttributes = OtherActor->GetComponentByClass<UMAttributeComponent>(); if (GEngine)
	if (hitAttributes != nullptr)
	{
		if (Hit.BoneName == "Head")
		{
			// Headshot
			hitAttributes->ApplyChange(healthTag, -(2 * DamageValue));
			// Add to alter
			UMAttributeComponent* shooterAttributes = GetInstigator()->GetComponentByClass<UMAttributeComponent>();
			if (shooterAttributes != nullptr)
			{
				shooterAttributes->ApplyChange(alterTag, 1);
			}
		}
		else
		{
			// Bodyshot
			hitAttributes->ApplyChange(healthTag, -DamageValue);
		}
	}
	Destroy();
}
