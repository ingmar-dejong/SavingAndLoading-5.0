// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"


class UAnimMontage;
/**
 * 
 */
UCLASS()
class SAVINGANDLOADING_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
protected:

	/* Sphere radius of the sweep to find desired target under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category = "Targeting")
	float SweepRadius;

	/* Fallback distance when sweep finds no collision under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category = "Targeting")
	float SweepDistanceFallback;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	int32 BlackholeCost; 

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY()
	bool bScreenToWorld;

public:

	virtual void StartAction_Implementation(AActor* Instigator)override;

	USAction_ProjectileAttack();
	
};
