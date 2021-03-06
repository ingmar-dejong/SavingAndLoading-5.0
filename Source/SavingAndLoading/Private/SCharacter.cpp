// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "Animation/AnimMontage.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Private/KismetTraceUtils.h"
#include "SActionComponent.h"
#include "SActionEffect.h"
#include "SPhysicalAnimationComponent.h"
#include "SActionEffect_Thorns.h"

static TAutoConsoleVariable<bool> CVarDebugDrawProjectile(TEXT("su.ProjectileDebugDraw"), false, TEXT("Enable Debug Lines for Projectile Component"), ECVF_Cheat);

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));
	PhysAnimComp = CreateDefaultSubobject<USPhysicalAnimationComponent>(TEXT("PhysicalAnimComponent"));
	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	TimeToHitParaName = "TimeToHit";
	RagePerHit = 5;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}



// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
// 	if (ActionComp)
// 	{
// 		ActionComp->AddAction(this, ThornsActionClass);
// 	}
	
}



// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::DashTeleport);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);

}

void ASCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHeatlhChange(this, Amount);
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
	ActionComp->StartActionByName(this, "Interacted");
}


void ASCharacter::DashTeleport()
{
	ActionComp->StartActionByName(this, "DashAttack");
}


void ASCharacter::JumpStart()
{
	bPressedJump = true;

}

void ASCharacter::JumpEnd()
{
	bPressedJump = false;
}

void ASCharacter::SecondaryAttack()
{
	ActionComp->StartActionByName(this, "SecondaryAttack");
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}

}
void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	OwningComp->ApplyRageChange(InstigatorActor, RagePerHit);

	// Damaged
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParaName, GetWorld()->TimeSeconds);
	}

	// Died
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		PhysAnimComp->Ragdoll();
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
	
}

void ASCharacter::OnRageChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewRage, float Delta)
{

	if (NewRage == OwningComp->GetMaxRage())
	{
		return;
	}
}


void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

