// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MActionComponent.h"
#include "MAttributeComponent.h"
#include "Revolver.generated.h"

UCLASS()
class MINIKILL_API ARevolver : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;
	
public:
	// Sets default values for this actor's properties
	ARevolver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	class UMActionComponent* ActionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	class UMAttributeComponent* AttributeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Extras, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1.0f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
