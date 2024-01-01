// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTaskRangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "SAICharacter.h"
#include "Revolver.h"
#include "Sabre.h"
#include "GameplayTagsModule.h"
#include "Components/ArrowComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTaskRangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();
	static FGameplayTag fireTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Fire");
	static FGameplayTag slashTag = UGameplayTagsManager::Get().RequestGameplayTag("Actions.Slash");
	if (ensure(controller))
	{
		ASAICharacter* aiCharacter = Cast<ASAICharacter>(controller->GetPawn());
		if (aiCharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		ARevolver* revolver = aiCharacter->GetRevolver();
		if (revolver != nullptr)
		{
			revolver->ActionComponent->StartAction(aiCharacter, fireTag);
			return EBTNodeResult::Succeeded;
		}

		ASabre* sabre = aiCharacter->GetSabre();
		if (sabre != nullptr)
		{
			sabre->ActionComponent->StartAction(aiCharacter, slashTag);
			return EBTNodeResult::Succeeded;
		}


		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack task failed!"));
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
