// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"
#include "../SavingAndLoading.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_Stanford);


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
 
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

// 	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);



	// Draw All Actions
 	for (USAction* Action : Actions)
 	{
 		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
 		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
 
 		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
 	}

}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;

}
void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	USAction* NewAction =  NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
// 		if (NewAction->bAutoStart == false && ensure(NewAction->CanStart(Instigator)))
// 		{
// 			NewAction->StartAction(Instigator);
// 		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(ActionToRemove);
	
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

// 			Is Client? To avoid infinite loop, in dit vb dus geen auth betekent dus dat je de client bent. Wel auth dan ben je de server.
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			

			// Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				// Is Client?
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}
			
		}
	}

	return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags); // WroteSomething = WroteSomething | Channel->ReplicateSubobject(Action, *Bunch, *RepFlags)
		}
	}

	return WroteSomething;
}



void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}


