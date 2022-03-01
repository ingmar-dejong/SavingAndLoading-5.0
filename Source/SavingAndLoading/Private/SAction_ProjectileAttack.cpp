// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "muzzle_01";
	AttackAnimDelay = 0.2f;
}


void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		// ** TODO: Particle Efefct //


		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);

	}

	
	
}


void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{

	
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	FTransform SpawnTM;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ CrosshairWorldPosition + CrosshairWorldDirection * 10000.f };

		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation("muzzle_01");
		FVector EndPoint{ End };


		TArray<FHitResult> Hits;
		float Radius = 30.f;
		FCollisionShape Shape;
		Shape.SetSphere(Radius);
		bool bBlockingHit = GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Visibility, Shape);   //(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
		FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;



		for (FHitResult Hit : Hits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor != InstigatorCharacter)
			{
				if (HitActor)
				{
					EndPoint = Hit.Location;
				}
			}


			//EndPoint = ScreenTraceHit.Location;
		}



		//find new direction/rotation from Hand pointing to impact point in world.
		FRotator ProjRotation = (EndPoint - HandLocation).Rotation();

		SpawnTM = FTransform(ProjRotation, HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

	}

	StopAction(InstigatorCharacter);
	
}
