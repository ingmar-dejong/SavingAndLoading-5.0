// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SAVINGANDLOADING_API ASDashProjectile : public AProjectileBase
{
	GENERATED_BODY()

protected:
	

	virtual void Explode_Implementation() override;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	FTimerHandle TimerHandle_DelayedDetonate;

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:
	ASDashProjectile();
	
};
