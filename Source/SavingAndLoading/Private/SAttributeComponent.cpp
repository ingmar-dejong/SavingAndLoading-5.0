// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
}

bool USAttributeComponent::IsAlive()
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHeatlhChange(float Delta)
{
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;

	Health = NewHealth;

	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	return true;
}


