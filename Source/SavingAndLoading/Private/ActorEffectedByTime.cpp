// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEffectedByTime.h"

// Sets default values
AActorEffectedByTime::AActorEffectedByTime()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AActorEffectedByTime::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorEffectedByTime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorEffectedByTime::OnActorLoaded_Implementation()
{

}

void AActorEffectedByTime::Interact_Implementation(APawn* InstigatorPawn)
{

}

