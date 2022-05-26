// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "ActorEffectedByTime.generated.h"

UCLASS()
class SAVINGANDLOADING_API AActorEffectedByTime : public AActor, public ISGameplayInterface

{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorEffectedByTime();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	
};
