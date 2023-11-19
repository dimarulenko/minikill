#include "MActionComponent.h"

UMActionComponent::UMActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UMAction> action : DefaultActions) {
		AddAction(GetOwner(), action);
	}
}

void UMActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Stop all
	TArray<UMAction*> actions_copy = Actions;
	for (UMAction* action : actions_copy)
	{
		if (action && action->IsRunning())
		{
			action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UMActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMActionComponent::AddAction(AActor* Instagator, TSubclassOf<UMAction> ActionClass)
{
	if (!ensure(ActionClass)) {
		return;
	}
	UMAction* new_action = NewObject<UMAction>(GetOwner(), ActionClass);
	if (ensure(new_action)) {
		new_action->Initialize(this);
		Actions.Add(new_action);
		if (new_action->AutoStart && ensure(new_action->CanStart(Instagator))) {
			new_action->StartAction(Instagator);
		}
	}
}

void UMActionComponent::RemoveAction(UMAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning())) return;
	Actions.Remove(ActionToRemove);
}

UMAction* UMActionComponent::GetAction(TSubclassOf<UMAction> ActionClass) const
{
	int index = -1;
	UMAction* action = nullptr;
	Actions.FindItemByClass(&action, &index, 0);
	return action;
}

bool UMActionComponent::StartAction(AActor* Instigator, FGameplayTag ActionTag)
{
	for (UMAction* action : Actions) {
		if (action && action->ActivationTag == ActionTag) {
			if (!action->CanStart(Instigator)) {
				FString msg = FString::Printf(TEXT("Failed to start: %s"), *ActionTag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, msg);
				continue; // If not for this line, FindByPredicate would be equivalent
			}
			action->StartAction(Instigator);
			return true;
		}
	}
	//UMAction** potential = Actions.FindByPredicate([=](UMAction* act) {return act->ActivationTag == ActionTag; });
	//if (potential) {
	//	UMAction* action = *potential;
	//		if (!action->CanStart(Instigator)) {
	//			FString msg = FString::Printf("Failed to start: %s", *ActionTag.ToString());
	//			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, msg);
	//		}
	//		action->StartAction(Instigator);
	//		return true;
	//}
	return false;
}

bool UMActionComponent::StopAction(AActor* Instigator, FGameplayTag ActionTag)
{
	UMAction** potential = Actions.FindByPredicate([=](UMAction* act) {return act->ActivationTag == ActionTag; });
	if (potential) {
		UMAction* action = *potential;
		action->StopAction(Instigator);
		return true;
	}
	return false;
}

