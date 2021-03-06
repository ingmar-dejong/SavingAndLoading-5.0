// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "SAttributeComponent.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));


	TargetPitch = 110.f;

	SetReplicates(true);
}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	AActor* Actor = this;

// 	USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(Actor->GetComponentByClass(USAttributeComponent::StaticClass()));
// 	
// 	if (AttributeComp)
// 	{
// 		AttributeComp->ApplyStateChange(Actor, BaseMesh, );
// 	}
			
	

	
	// OnRep_Notfiys worden in c++ by default alleen gerunt op de client en niet op de server. Voor Server dus de OnRep_Notfiy expliciet nog callen
	// In BP worden ze wel automatisch allebei (Server & Client) gerunt!
	OnRep_LidOpened();  
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened);
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrentPitch = bLidOpened ? TargetPitch : 0.0f; // Use TargetPitch en anders 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}

// void ASItemChest::OnTimeStateChanged(AActor* OwningActor, USAttributeComponent* OwningComp, int32 NewState, int32 Delta)
// {
// 	UE_LOG(LogTemp, Log, TEXT("State %i", NewState));
// }

