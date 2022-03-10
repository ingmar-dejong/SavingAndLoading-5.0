// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "SPhysicalAnimationComponent.generated.h"

class USkeletalMeshComponent;
class USkeletalMesh;
class USPhysicalAnimationComponent;
class ACharacter;




UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAVINGANDLOADING_API USPhysicalAnimationComponent : public UPhysicalAnimationComponent
{
	GENERATED_BODY()
	

public:
	USPhysicalAnimationComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void HitReactionCall(FHitResult AimHit);

	UFUNCTION(BlueprintCallable, Category = "PhysicallAnimFunctions")
	static USPhysicalAnimationComponent* GetPhysicallAnimFunctions(AActor* FromActor);

	


protected:

	virtual void BeginPlay() override;
	UPROPERTY()
	FName BoneName = "pelvis";
	
	UPROPERTY()
	USkeletalMeshComponent* MyCharMesh;
	
	UPROPERTY()
	USkeletalMesh* NewMesh;

	UFUNCTION()
	void TogglePhyscialAnimation();

	UPROPERTY()
	ACharacter* TargetCharacter;

	UPROPERTY()
	float HitReactionTimeRemaining;
	

 
};
