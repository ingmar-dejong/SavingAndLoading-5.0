// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"
#include "SGameplayInterface.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"


// Sets default values
AInteractableActorBase::AInteractableActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	
	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("Action Component"));
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));
	BaseMeshState = CreateDefaultSubobject<UStaticMesh>(TEXT("BaseMesh State"));
	InteractedMeshState = CreateDefaultSubobject<UStaticMesh>(TEXT("Interacted Mesh State"));

	MeshStages.Add(BaseMeshState);
	MeshStages.Add(InteractedMeshState); 
}

void AInteractableActorBase::OnActorLoaded_Implementation()
{

}

void AInteractableActorBase::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

// Called when the game starts or when spawned
void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

