// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthPotion = CreateDefaultSubobject<UStaticMeshComponent>("Health Potion Mesh");
	RootComponent = HealthPotion;

	SphereComp = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	SphereComp->SetupAttachment(HealthPotion);
}



void ASPowerup_HealthPotion::HideAndShowPowerUp()
{

}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp)&& !AttributeComp->IsFullHealth())
	{
		AttributeComp->ApplyHeatlhChange(this, AttributeComp->GetMaxHealth());
		Destroy();
	}


}
// Called when the game starts or when spawned

void ASPowerup_HealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPowerup_HealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

