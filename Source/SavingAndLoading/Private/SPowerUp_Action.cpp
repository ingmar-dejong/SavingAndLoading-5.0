// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp_Action.h"
#include "SAction.h"
#include "SActionComponent.h"

void ASPowerUp_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}
	USActionComponent* ActionComp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			//UE_LOG(LogTemp, Log, TEXT("Instigator already has action of class: %s"), *GetNameSafe(ActionToGrant));
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerup();
		UE_LOG(LogTemp, Log, TEXT("Action granted: %s"), *GetNameSafe(ActionToGrant));
	}
	

}
