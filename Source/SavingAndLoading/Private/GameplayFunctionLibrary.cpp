// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFunctionLibrary.h"
#include "SAttributeComponent.h"
#include "SPhysicalAnimationComponent.h"


bool UGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	
	if (AttributeComp)
	{
		return AttributeComp->ApplyHeatlhChange(DamageCauser, -DamageAmount);
	 
	}
	return false;
}

bool UGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		USPhysicalAnimationComponent* PhysComp = PhysComp->GetPhysicallAnimFunctions(TargetActor);
		if (PhysComp)
		{
			
				PhysComp->HitReactionCall(HitResult);
				USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
				UPrimitiveComponent* HitComp = HitResult.GetComponent();
				if (AttributeComp->IsActorAlive(TargetActor))
				{
					//Direction = Target - Origin
					FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
					Direction.Normalize();
					HitComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
				}
		}
  		
		return true;
	}
	return false;
}
 