// Fill out your copyright notice in the Description page of Project Settings.


#include "AMagicProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SAttributeComponent.h"
#include "GameplayFunctionLibrary.h"
#include "SActionComponent.h"
#include "SPhysicalAnimationComponent.h"
#include "SActionEffect.h"



// Sets default values
AAMagicProjectile::AAMagicProjectile()
{
	SphereComp->SetSphereRadius(20.f);
	InitialLifeSpan = 10.f;
	DamageAmount = 20.f;
}

void AAMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hotreload)
	// PostInitializeComponent is the preferred way of binding any events.
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAMagicProjectile::OnActorOverlap);
}


void AAMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
 	if (OtherActor && OtherActor != GetInstigator())
 	{

		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if(UGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}


// 		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
//  		if (AttributeComp)
//  		{
//  			AttributeComp->ApplyHeatlhChange(GetInstigator(), -DamageAmount);
//  
//  			Destroy();
//  		}
	}

	

}


