// Fill out your copyright notice in the Description page of Project Settings.


#include "SPhysicalAnimationComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "SAttributeComponent.h"




USPhysicalAnimationComponent::USPhysicalAnimationComponent()
{
	
	HitReactionTimeRemaining = 0.0f;
}

void USPhysicalAnimationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DeltaTime = DeltaTime * 0.8f;
	float Interp = FMath::FInterpTo(HitReactionTimeRemaining, 0.f, DeltaTime, 0.5f);
	HitReactionTimeRemaining = Interp - DeltaTime;
	TargetCharacter = Cast<ACharacter>(GetOwner());
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetCharacter);

	UE_LOG(LogTemp, Log, TEXT("%f"), HitReactionTimeRemaining);
	if (!AttributeComp->IsAlive())
	{
		SetComponentTickEnabled(false);
		return;
	}
	if (HitReactionTimeRemaining <= 0.0f)
	{
		HitReactionTimeRemaining = 0.0f;
		MyCharMesh->SetAllBodiesBelowSimulatePhysics(BoneName, false, true);
		SetComponentTickEnabled(false); 
	}
	else
	{
		MyCharMesh->SetAllBodiesBelowPhysicsBlendWeight(BoneName, HitReactionTimeRemaining - 1.0f, false, true);
	}

}

void USPhysicalAnimationComponent::HitReactionCall(FHitResult AimHit)
{
	
		SetComponentTickEnabled(true);
		FVector Direction = AimHit.TraceEnd - AimHit.TraceStart;
		HitReactionTimeRemaining = (HitReactionTimeRemaining + 1.5f);

		this->ApplyPhysicalAnimationProfileBelow(BoneName, "Strong", false, false);

		if (MyCharMesh)
		{			
			MyCharMesh->SetAllBodiesBelowSimulatePhysics(BoneName, true, false);
			Direction.Normalize();
			MyCharMesh->AddImpulseAtLocation(Direction * 50000.f, AimHit.Location, BoneName);
		}

		return;
}

void USPhysicalAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	TargetCharacter = Cast<ACharacter>(GetOwner());
	if (TargetCharacter)
	{
		MyCharMesh = TargetCharacter->GetMesh();
		if (MyCharMesh)
		{
			this->SetSkeletalMeshComponent(MyCharMesh);
		}
		
	}


}
USPhysicalAnimationComponent* USPhysicalAnimationComponent::GetPhysicallAnimFunctions(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USPhysicalAnimationComponent>(FromActor->GetComponentByClass(USPhysicalAnimationComponent::StaticClass()));
	}

	return nullptr;
}

void USPhysicalAnimationComponent::TogglePhyscialAnimation()
{
	
}



