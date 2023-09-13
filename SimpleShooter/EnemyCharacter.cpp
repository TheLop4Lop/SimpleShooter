// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "HealthComponent.h"
#include "BaseShield.h"

#include "BasicEnemyController.h"

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    HealthComponent = FindComponentByClass<UHealthComponent>();

    AIController = Cast<ABasicEnemyController>(GetController());

	if(AIController != nullptr)
	{
		AIController->EnemyCameraZone = CharacterCameraZone;
	}

    SpawnInventory();

    Shield = GetWorld()->SpawnActor<ABaseShield>(ShieldClass);
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ShieldSpawn"));
	Shield->SetOwner(this);

	Shield->SetShieldOff();

	bDoOnceShieldOn = true;
	bDoOnceShieldOff = true;

}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FallingDamage();

	if(Aiming || AimingRifle && GetCharacterMovement()->MaxWalkSpeed > 610)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}

	ShieldActivation();

}

//Character Health and Damage
float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageIn = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if(HealthComponent != nullptr)
	{
		if(AIController != nullptr)
		{
			AIController->PlayerDetectedDamage = true;
			AIController->DoOnce = true;
		}
		HealthComponent->SetDamage(DamageIn);
		GettingDamage.Broadcast();

		if(HealthComponent->IsDead())
		{
			bDeath = true;
			/*ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
			if(GameMode !=nullptr)
			{
				GameMode->PawnKilled(this);
			}
			*/
			DetachFromControllerPendingDestroy();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	return DamageIn;
}

void AEnemyCharacter::StarAim()
{
    Super::StarAim();
    
	Aiming = true;
	
}

void AEnemyCharacter::StopAim()
{
    Super::StopAim();

	Aiming = false;
	AimingRifle = false;

}

void AEnemyCharacter::ShieldOn()
{
    Super::ShieldOn();

	if(ShieldClass != nullptr)
	{
		Shield->SetShieldActive();

		Aiming = false;
		AimingRifle = false;
		bDoOnceShieldOff = true;

		Shield->ReduceLifeTime();
	}

}

void AEnemyCharacter::ShieldOff()
{
    Super::ShieldOff();

	if(Shield != nullptr)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;

		Shield->SetShieldOff();

		bDoOnceShieldOn = true;
	}

}

void AEnemyCharacter::ShieldActivation()
{
    Super::ShieldActivation();
    
	if(bShieldOrder && GetCharacterMovement()->GetCurrentAcceleration().Size() == 0 
		&& bDoOnceShieldOn && Shield->bOverHeat == false)
	{
		ShieldOn();
		bDoOnceShieldOn = false;
	}else if(bShieldOrder && GetCharacterMovement()->GetCurrentAcceleration().Size() != 0 
		&& bDoOnceShieldOff && bDoOnceShieldOn == false)
	{
		ShieldOff();
		bDoOnceShieldOff = false;
	}else if(bShieldOrder == false && bDoOnceShieldOff == false)
	{
		bDoOnceShieldOff = true;
	}

}