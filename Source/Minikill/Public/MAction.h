// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "MAction.generated.h"

class UMActionComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class MINIKILL_API UMAction : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UMActionComponent* comp);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* instigator);

	UPROPERTY(EditDefaultsOnly, Category="Action")
	FGameplayTag ActivationTag;

	UPROPERTY(EditDefaultsOnly, Category="Action")
	bool AutoStart;

	UPROPERTY()
	UMActionComponent* ActionComponent;

protected:

	UFUNCTION(BlueprintCallable, Category="Action")
	UMActionComponent* GetOwningComponent() const;

	// Gives these tags when starts, removes when stops
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer GrantsTags;

	// Won't start if owning actor has any of these
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
	
};
