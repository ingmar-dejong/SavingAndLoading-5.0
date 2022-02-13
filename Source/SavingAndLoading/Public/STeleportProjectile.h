// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SAVINGANDLOADING_API ASTeleportProjectile : public AProjectileBase
{
	GENERATED_BODY()
public:
	ASTeleportProjectile();

protected:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;
};
