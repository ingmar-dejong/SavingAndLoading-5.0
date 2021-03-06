// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerUpActor.h"
#include "SPowerup_HealthPotion.generated.h"

class USAttributeComponent;
class USphereComponent;

UCLASS()
class SAVINGANDLOADING_API ASPowerup_HealthPotion : public ASPowerUpActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerup_HealthPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int32 CreditCost;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

};
