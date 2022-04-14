// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
ASPowerUpActor::ASPowerUpActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	RootComponent = SphereComp;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 5.f;
	
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPowerUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPowerUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPowerUpActor::Interact_Implementation(APawn* InstigatorPawn)
{
	

}

void ASPowerUpActor::ShowPowerup()
{
	SetPowerupState(true);
}



void ASPowerUpActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandel_RespawnTimer, this, &ASPowerUpActor::ShowPowerup, RespawnTime);
}

void ASPowerUpActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	RootComponent->SetVisibility(bNewIsActive, true);
}
