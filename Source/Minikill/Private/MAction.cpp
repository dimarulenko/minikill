// Fill out your copyright notice in the Description page of Project Settings.


#include "MAction.h"
#include <Logging/StructuredLog.h>
#include "MActionComponent.h"

void UMAction::Initialize(UMActionComponent* comp)
{
	ActionComponent = comp;
}

bool UMAction::IsRunning() const
{
	return bIsRunning;
}


bool UMAction::CanStart_Implementation(AActor* instigator)
{
	if (bIsRunning) return false;
	UMActionComponent* comp = GetOwningComponent();
	ensureAlways(comp);
	if (comp->ActiveGameplayTags.HasAny(BlockedTags))
		return false;
	return true;
}


void UMAction::StartAction_Implementation(AActor* instigator) {

	// For some reason this log crashes the game
	//UE_LOGFMT(LogTemp, Log, "Started: {}", GetName());
	UMActionComponent* comp = GetOwningComponent();
	ensureAlways(comp);
	comp->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
	comp->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void UMAction::StopAction_Implementation(AActor* instigator)
{
	// For some reason this log crashes the game
	//UE_LOGFMT(LogTemp, Log, "Started: {}", GetName());
	UMActionComponent* comp = GetOwningComponent();
	ensureAlways(comp);
	comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRunning = false;
	comp->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

UMActionComponent* UMAction::GetOwningComponent() const
{
	return ActionComponent;
}
