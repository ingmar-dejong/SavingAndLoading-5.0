// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerUpActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SAVINGANDLOADING_API ASPowerUpActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ShowPowerup();

	void SetPowerupState(bool bNewIsActive);

	void HideAndCooldownPowerup();
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;
	
	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	FTimerHandle TimerHandel_RespawnTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn) override;

};
