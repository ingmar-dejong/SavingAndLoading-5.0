// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "GameplayTagContainer.h"
#include "InteractableActorBase.generated.h"


class UStaticMeshComponent;
class USAttributeComponent;
class USActionComponent;
class UStaticMesh;

UCLASS()
class SAVINGANDLOADING_API AInteractableActorBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActorBase();

	void Interact_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* BaseMeshState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* InteractedMeshState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh States")
	TArray<class UStaticMesh*> MeshStages; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bIsInteractedWith;

	UPROPERTY(EditDefaultsOnly, SaveGame, Category = "Tags")
	FGameplayTag InteractedTag;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
