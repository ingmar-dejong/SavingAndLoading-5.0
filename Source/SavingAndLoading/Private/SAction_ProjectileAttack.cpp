// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "muzzle_01";
	AttackAnimDelay = 0.2f;

	SweepRadius = 20.0f;
	SweepDistanceFallback = 5000;
}


void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		if (Character->HasAuthority())
		{
			// ** TODO: Particle Efefct /
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
		

	}

	
	
}


void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{

// 	if (ProjectileClass)
// 	{
// 	
// 		FVector2D ViewPortSize;
// 		if (GEngine && GEngine->GameViewport)
// 		{
// 			GEngine->GameViewport->GetViewportSize(ViewPortSize);
// 		}
// 
// 		FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
// 		FVector CrosshairWorldPosition;
// 		FVector CrosshairWorldDirection;
// 		FTransform SpawnTM;
// 
// 		bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetControl, CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);
// 
// 		if (bScreenToWorld)
// 		{
// 			FHitResult ScreenTraceHit;
// 			const FVector Start{ CrosshairWorldPosition };
// 			const FVector End{ CrosshairWorldPosition + CrosshairWorldDirection * 10000.f };
// 
// 			FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation("muzzle_01");
// 			FVector EndPoint{ End };
// 
// 
// 			TArray<FHitResult> Hits;
// 			float Radius = 30.f;
// 			FCollisionShape Shape;
// 			Shape.SetSphere(Radius);
// 			bool bBlockingHit = GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Visibility, Shape);   //(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
// 			FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
// 
// 			for (FHitResult Hit : Hits)
// 			{
// 				AActor* HitActor = Hit.GetActor();
// 				if (HitActor != InstigatorCharacter)
// 				{
// 					if (HitActor)
// 					{
// 						EndPoint = Hit.Location;
// 					}
// 				}
// 
// 
// 				//EndPoint = ScreenTraceHit.Location;
// 			}
// 
// 
// 
// 			//find new direction/rotation from Hand pointing to impact point in world.
// 			FRotator ProjRotation = (EndPoint - HandLocation).Rotation();
// 
// 			SpawnTM = FTransform(ProjRotation, HandLocation);
// 			FActorSpawnParameters SpawnParams;
// 			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// 			SpawnParams.Instigator = InstigatorCharacter;
// 			GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
// 		}
// 	}.// old here
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		// We trace against the environment first to find whats under the player crosshair.
		// We use the hit location to adjust the projectile launch direction so it will hit what is under the crosshair rather than shoot straight forward from the player hands.

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(SweepRadius);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();

		// Add sweep radius onto start to avoid the sphere clipping into floor/walls the camera is directly against.
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
		FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);

		FHitResult Hit;
		// returns true if we got to a blocking hit (Channel1="Projectile" defined in DefaultGame.ini)
		if (GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ECC_GameTraceChannel1, Shape, Params))
		{
			// Overwrite trace end with impact point in world
			TraceEnd = Hit.ImpactPoint;
		}

		// find new direction/rotation from Hand pointing to impact point in world.
		FRotator ProjRotation = (TraceEnd - HandLocation).Rotation();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
	
}


