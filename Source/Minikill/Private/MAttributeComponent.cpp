// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeComponent.h"


// Sets default values for this component's properties
UMAttributeComponent::UMAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto [key, val] : DefaultAttributes) {
		AddAttribute(key, val);
	}
}

void UMAttributeComponent::AddAttribute(FGameplayTag Attribute, float DefaultValue)
{
	AttributeMap.Add(Attribute, DefaultValue);
}

void UMAttributeComponent::ApplyChange(FGameplayTag Attribute, float Delta)
{
	if (AttributeMap.Contains(Attribute)) {
		AttributeMap[Attribute] += Delta;
		OnAttributeChanged.Broadcast(GetOwner(), this, Attribute, AttributeMap[Attribute], Delta);
	}
	else {
		FString msg = FString::Printf(TEXT("Failed to apply change to because the attribute component doesn't have it: %s"), *Attribute.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, msg);
	}
}

float UMAttributeComponent::GetAttribute(FGameplayTag Attribute) const
{
	return AttributeMap[Attribute];
}

bool UMAttributeComponent::TryGetAttribute(FGameplayTag Attribute, float& OutValue) const
{
	if (!AttributeMap.Contains(Attribute)) return false;
	OutValue = AttributeMap[Attribute];
	return true;
}
