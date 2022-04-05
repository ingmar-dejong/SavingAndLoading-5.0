// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAttributeComponent.h"

void UBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
// 		AActor* SelfActor = Cast<AActor>(BlackboardComp->GetValueAsObject("SelfActor"));
// 		if (SelfActor)
// 		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					USAttributeComponent* AttributeComp = AttributeComp->GetAttributes(AIPawn);
					bool bIsLowHealth = false;
					if (ensure(AttributeComp))
					{
						
						if (AttributeComp->GetHealth() <= 50.f)
						{
							UE_LOG(LogTemp, Warning, TEXT("AI Health is: %f , HIDE!"), AttributeComp->GetHealth()); 
							bIsLowHealth = true;
						}
					}
				BlackboardComp->SetValueAsBool(GoHide.SelectedKeyName, bIsLowHealth);
				}
			}
		//}
	}
}
