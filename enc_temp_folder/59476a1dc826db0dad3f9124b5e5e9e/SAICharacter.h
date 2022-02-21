// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;

UCLASS()
class SAVINGANDLOADING_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParaName;

	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Components")
	USAttributeComponent* AttributeComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn); 

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
