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

static TAutoConsoleVariable<bool> CVarDebugDrawProjectile(TEXT("su.ProjectileDebugDraw"), false, TEXT("Enable Debug Lines for Projectile Component"), ECVF_Cheat);

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	TimeToHitParaName = "TimeToHit";

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

	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttackTimeElapsed, 0.2f);
}
	
void ASCharacter::PrimaryAttackTimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ASCharacter::DashTeleport()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::DashTimeElapsed, 0.2f);
}

void ASCharacter::DashTimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}


FTransform ASCharacter::GetLookDirection()
{

	bool bDebugDraw = CVarDebugDrawProjectile.GetValueOnGameThread();

	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	FTransform SpawnTM;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ CrosshairWorldPosition + CrosshairWorldDirection * 10000.f };

		FVector HandLocation = GetMesh()->GetSocketLocation("muzzle_01");
		FVector EndPoint{ End };

	
		TArray<FHitResult> Hits;
		float Radius = 30.f;
		FCollisionShape Shape;
		Shape.SetSphere(Radius);
		bool bBlockingHit = GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Visibility, Shape);   //(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
		FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;


		
		for (FHitResult Hit : Hits)
		{
			if (bDebugDraw)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.f);
			}
			
			AActor* HitActor = Hit.GetActor();
			if (HitActor != this)
			{
					if (HitActor)
					{
						EndPoint = Hit.Location;
					}
			}
			

			//EndPoint = ScreenTraceHit.Location;
		}

	

		//find new direction/rotation from Hand pointing to impact point in world.
		FRotator ProjRotation = (EndPoint - HandLocation).Rotation();

		SpawnTM = FTransform(ProjRotation, HandLocation);

		
	}
	return SpawnTM;
}



void ASCharacter::SpawnProjectile(TSubclassOf<AActor>(ClassToSpawn))
{
	FTransform GetLookDir = GetLookDirection();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<AActor>(ClassToSpawn, GetLookDir, SpawnParams);
}

void ASCharacter::JumpStart()
{
	bPressedJump = true;

}

void ASCharacter::JumpEnd()
{
	bPressedJump = false;
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
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParaName, GetWorld()->TimeSeconds);
	}

	if (NewHealth<= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
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

