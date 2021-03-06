// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SActionComponent.h"
#include "SPhysicalAnimationComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

    AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

    ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComponent");

    PhysAnimComp = CreateDefaultSubobject<USPhysicalAnimationComponent>(TEXT("Physical Animation Component"));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);

    TargetActorKey = "TargetActor";

    TimeToHitParaName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

}


void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    // Died
    if (Delta < 0.f)
    {
        if (InstigatorActor != this) // Not checking if its not another AI, so they can aggro eachother 
        {
            SetTargetActor(InstigatorActor);
        }

        if (ActiveHealthBar == nullptr)
        {
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
                ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
        }

        GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParaName, GetWorld()->TimeSeconds);
        USPhysicalAnimationComponent* PhysAnimComponent = USPhysicalAnimationComponent::GetPhysicallAnimFunctions(this);
        if (NewHealth <= 0)
        {
            // Stop BT
            AAIController* AIC = Cast<AAIController>(GetController());
            if (AIC)
            {
                AIC->GetBrainComponent()->StopLogic("AI Killed"); 
            }
            
            PhysAnimComponent->Ragdoll();
//      
// 
//             // Raddoll
//             GetMesh()->SetAllBodiesSimulatePhysics(true);
// 
//             GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//             GetCharacterMovement()->DisableMovement();

            // Set LifeSpawn
            SetLifeSpan(10.f);
        }
    }
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
    if (AIC)
    {
       AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
    }
}


AActor* ASAICharacter::GetTargetActor() const
{
    AAIController* AIC = Cast<AAIController>(GetController());
        if (AIC)
        {
            return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
        }

        return nullptr;
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
   if (GetTargetActor() != Pawn)
   {
       SetTargetActor(Pawn);
	   
       MulticastPawnSeen();
   }
    
    //DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
 
}

void ASAICharacter::MulticastPawnSeen_Implementation()
{
	USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (NewWidget)
	{
		NewWidget->AttachedActor = this;
		NewWidget->AddToViewport(10);
	}
}
