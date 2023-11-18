// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MAction.h"
#include "MActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UMActionComponent*, OwningComp, UMAction*, Action);

//UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

UCLASS(Blueprintable)
class MINIKILL_API UMActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;

public:
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instagator, TSubclassOf<UMAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UMAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UMAction* GetAction(TSubclassOf<UMAction> ActionClass) const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartAction(AActor* Instigator, FGameplayTag ActionTag);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopAction(AActor* Instigator, FGameplayTag ActionTag);

	// Sets default values for this component's properties
	UMActionComponent();

protected:
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<UMAction>> DefaultActions;

	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<UMAction*> Actions;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
