// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "ProjectileBase.h"

ASDashProjectile::ASDashProjectile()
{
	DetonateDelay = 0.3f;
	TeleportDelay = 0.1f;
	MovementComp->InitialSpeed = 6000.f;
	SphereComp->SetSphereRadius(20.f);

}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);

}

void ASDashProjectile::TeleportInstigator()
{
	if (IsPendingKill())
	{
			AActor* ActorToTeleport = GetInstigator();
			if (ensure(ActorToTeleport))
			{
				ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
			}

			Destroy();
		
	}
	else
	{
		AActor* ActorToTeleport = GetInstigator();
		if (ensure(ActorToTeleport))
		{
			ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
		}

		Destroy();
	}
	
}


