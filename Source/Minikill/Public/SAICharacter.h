// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class ARevolver;
class UPawnSensingComponent;

UCLASS()
class MINIKILL_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARevolver> RevolverBP;

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensing;

	ARevolver* Revolver;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

public:

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	class UMAttributeComponent* AttributeComponent;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	ARevolver* GetRevolver() const { return Revolver; }


};
