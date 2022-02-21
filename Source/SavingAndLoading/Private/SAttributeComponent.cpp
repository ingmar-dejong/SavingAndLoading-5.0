// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"



// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}



float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

bool USAttributeComponent::ApplyHeatlhChange(AActor* InstigatorActor ,float Delta)
{
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;

	Health = NewHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	return true;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor); // USAttributeComponent::GetAttributes(Actor) hoeft niet omdat we al in het juiste object zitten.
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	
	return false;
}
