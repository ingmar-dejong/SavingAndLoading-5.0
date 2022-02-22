// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_HealSelf.h"
#include "SAttributeComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackBoardComponent.h"


EBTNodeResult::Type UBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
		APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
		if (ensure(AttributeComp))
		{
			AttributeComp->ApplyHeatlhChange(MyPawn, AttributeComp->GetMaxHealth());
		}

		return EBTNodeResult::Succeeded;
}
