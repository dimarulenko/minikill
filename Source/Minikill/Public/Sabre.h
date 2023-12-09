// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MActionComponent.h"
#include "MAttributeComponent.h"
#include "Sabre.generated.h"

UCLASS()
class MINIKILL_API ASabre : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASabre();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	class UMActionComponent* ActionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	class UMAttributeComponent* AttributeComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
