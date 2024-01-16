// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class ARevolver;
class ASabre;
class UPawnSensingComponent;

UCLASS()
class MINIKILL_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARevolver> RevolverBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASabre> SabreBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float Range;

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(VisibleAnywhere)
	ARevolver* Revolver;
	UPROPERTY(VisibleAnywhere)
	ASabre* Sabre;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	UFUNCTION()
	void OnPawnHeard(APawn* Pawn, const FVector& Location, float Volume);

	virtual void PossessedBy(AController* NewController) override;

public:

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	class UMAttributeComponent* AttributeComponent;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	ARevolver* GetRevolver() const { return Revolver; }
	UFUNCTION(BlueprintCallable, Category = Weapon)
	ASabre* GetSabre() const { return Sabre; }


};
