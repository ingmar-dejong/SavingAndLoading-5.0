// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component"), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceDistance = 800.f;
	TraceRadius = 30.f;
	CollisionChannel = ECC_WorldDynamic;
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();


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
		AActor* MyOwner = GetOwner();
		FHitResult ScreenTraceHit;
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ CrosshairWorldPosition + CrosshairWorldDirection * TraceDistance };

		FVector EndPoint{ End };

		TArray<FHitResult> Hits;
		FCollisionShape Shape;
		Shape.SetSphere(TraceRadius);

		FCollisionObjectQueryParams ObjectQeuryParams;
		ObjectQeuryParams.AddObjectTypesToQuery(CollisionChannel);

		bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQeuryParams, Shape);
		GetWorld()->LineTraceSingleByObjectType(HitResultAttack, Start, End, ObjectQeuryParams);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f, 1.f);
		FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

		// Clear ref before trying to fill
		FocusedActor = nullptr;

		for (FHitResult Hit : Hits)
		{
			if (bDebugDraw)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.f);
			}

			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				if (HitActor->Implements<USGameplayInterface>()) // Hier "U " prefix omdat checkt of de hele Interface bestaat
				{
					FocusedActor = HitActor;
					break;
				}

			}

		}
		
		if (FocusedActor)
		{
			if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
			{
				DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
			}

			if (DefaultWidgetInstance)
			{
				DefaultWidgetInstance->AttachedActor = FocusedActor;

				if (!DefaultWidgetInstance->IsInViewport())
				{
					DefaultWidgetInstance->AddToViewport();
				}
				
			}

		}
		else
		{
			if (DefaultWidgetInstance)
			{
				DefaultWidgetInstance->RemoveFromParent();
			}
		}
		



	}
}



FHitResult USInteractionComponent::GetHitResult() const
{
	return HitResultAttack;
}

void USInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact");
		return;

	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn); // "I" prefix omdat je cast naar het level?
	

// 	AActor* MyOwner = GetOwner();
// 	
// 	FVector EyeLocation;
// 	FRotator EyeRotation;
// 	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
// 
// 	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
// 
// 
// 	//FHitResult Hit;
// 	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQeuryParams);
// 
// 	TArray<FHitResult> Hits;
// 	float Radius = 30.f;
// 	FCollisionShape Shape;
// 	Shape.SetSphere(Radius);
// 
// 	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQeuryParams, Shape);
// 	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
// 
// 	for (FHitResult Hit : Hits)
// 	{
// 		AActor* HitActor = Hit.GetActor();
// 		if (HitActor)
// 		{
// 			if (HitActor->Implements<USGameplayInterface>()) // Hier "U " prefix omdat checkt of de hele Interface bestaat
// 			{
// 				APawn* MyPawn = Cast<APawn>(MyOwner);
// 				ISGameplayInterface::Execute_Interact(HitActor, MyPawn); // "I" prefix omdat je cast naar het level?
// 				break;
// 			}
// 
// 		}
// 		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.f);
// 	}
// 
// 
// 
// 
// 	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f, 0, 2.f);


}