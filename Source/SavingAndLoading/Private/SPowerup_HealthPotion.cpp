// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"


// Lecture 26 Course 3 09:00
#define LOCTEXT_NAMESPACE "InteractableActors"


// Sets default values
ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	CreditCost = 50;
}

void ASPowerup_HealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPowerup_HealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth() )
	{
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHeatlhChange(this, AttributeComp->GetMaxHealth()))
			{
				HideAndCooldownPowerup();
			}
			
		}
	}
}

FText ASPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health");
	}

	return FText::Format(LOCTEXT( "HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum"), CreditCost);
}

#undef LOCTEXT_NAMESPACE
