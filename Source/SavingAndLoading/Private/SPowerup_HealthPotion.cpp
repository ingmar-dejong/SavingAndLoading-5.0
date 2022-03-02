// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void ASPowerup_HealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPowerup_HealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

