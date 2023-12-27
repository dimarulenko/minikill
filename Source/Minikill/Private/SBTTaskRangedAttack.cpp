// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTaskRangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "SAICharacter.h"
#include "Revolver.h"
#include "GameplayTagsModule.h"
#include "Components/ArrowComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


static FGameplayTag fireTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Fire");


EBTNodeResult::Type USBTTaskRangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	if (ensure(controller))
	{
		ASAICharacter* aiCharacter = Cast<ASAICharacter>(controller->GetPawn());
		if (aiCharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		ARevolver* revolver = aiCharacter->GetRevolver();
		if (revolver == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		revolver->ActionComponent->StartAction(aiCharacter, fireTag);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
