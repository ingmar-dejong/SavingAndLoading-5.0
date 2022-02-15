// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI Pawn and TargetActor
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			 AAIController* MyController =  OwnerComp.GetAIOwner();
			 if (ensure(MyController))
			 {
				 APawn* AIPawn = MyController->GetPawn();
				 if (ensure(AIPawn))
				 {
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWihtinAttackRange = DistanceTo < 2000.f;

					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWihtinAttackRange); 
				 }
			 }
		}
	}
}
