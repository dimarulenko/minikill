// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


void USBTService_CheckRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	if (ensure(blackboard))
	{
		AActor* target = Cast<AActor>(blackboard->GetValueAsObject("TargetActor"));
		if (target)
		{
			AAIController* owner = OwnerComp.GetAIOwner();
			if (ensure(owner))
			{
				APawn* AIPawn = owner->GetPawn();
				if (ensure(AIPawn))
				{
					float distance = FVector::Distance(target->GetActorLocation(), AIPawn->GetActorLocation());
					bool withinRange = distance < 2000.f;
					bool hasLOS = false;
					if (withinRange)
					{
						hasLOS = owner->LineOfSightTo(target);
					}
					blackboard->SetValueAsBool(RangeKey.SelectedKeyName, (withinRange && hasLOS));
				}
			}
		}
	}
}
