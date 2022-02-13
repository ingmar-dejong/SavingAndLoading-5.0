// Fill out your copyright notice in the Description page of Project Settings.


#include "AMagicProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SAttributeComponent.h"



// Sets default values
AAMagicProjectile::AAMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShpereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	ShpereComp->SetCollisionProfileName("Projectile");
	ShpereComp->OnComponentBeginOverlap.AddDynamic(this, &AAMagicProjectile::OnActorOverlap);
	RootComponent = ShpereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("ParctileSystemComponent");
	EffectComp->SetupAttachment(ShpereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 2000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

}

void AAMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHeatlhChange(-20.f);

			Destroy();
		}
	}

}

// Called when the game starts or when spawned
void AAMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

