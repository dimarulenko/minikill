// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <GameplayTagContainer.h>
#include "MAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnAttributeChanged, AActor*, InstigatorActor, UMAttributeComponent*, OwningComp, FGameplayTag, Attribute, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIKILL_API UMAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TMap<FGameplayTag, float> AttributeMap;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TMap<FGameplayTag, float> DefaultAttributes;

public:	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddAttribute(FGameplayTag Attribute, float DefaultValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyChange(FGameplayTag Attribute, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetAttribute(FGameplayTag Attribute) const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool TryGetAttribute(FGameplayTag Attribute, float& OutValue) const;
	// Sets default values for this component's properties
	UMAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged  OnAttributeChanged;
};
