// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class InputComponent; 
class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class USActionComponent;
class USPhysicalAnimationComponent;
class USActionEffect;
class USActionEffect_Thorns;

UCLASS()
class SAVINGANDLOADING_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParaName;

	/* Fallback distance when sweep finds no collision under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category = "Targeting")
	float SweepDistanceFallback;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float SweepRadius;

	int32 RagePerHit;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<USActionEffect_Thorns> ThornsActionClass;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	

public:
	// Sets default values for this character's properties
	ASCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void SprintStart();
	void SprintStop();
	void PrimaryAttack(); 
	void JumpStart();
	void JumpEnd();
	void SecondaryAttack();
	void PrimaryInteract();
	void DashTeleport();

	
	
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USPhysicalAnimationComponent* PhysAnimComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnRageChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewRage, float Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

};
