// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShield.h"
#include "HealthComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ABaseShield::ABaseShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root = RootComponent;

}

// Called when the game starts or when spawned
void ABaseShield::BeginPlay()
{
	Super::BeginPlay();
	
	SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();
	HealthComp = FindComponentByClass<UHealthComponent>();

	bInUse = false;
	DamageRate = 0.1f;
	LifeTime = MaxLifeTime;

}

// Called every frame
void ABaseShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HealthComp != nullptr)
	{
		if(HealthComp->IsDead())
		{
			DamageRate = 0.7f;
		}else
		{
			DamageRate = 0.1f;
		}
	}

}

float ABaseShield::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageIn = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageIn/=DamageIn;

	if(HealthComp != nullptr)
	{
		HealthComp->SetDamage(DamageIn);
	}

	return DamageIn;

}

void ABaseShield::ReduceLifeTime()
{	
	bInUse = true;

	ShieldAction.Broadcast();
	if(!bOverHeat)
	{
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABaseShield::ShieldLifeTimeDown, 0.1f, bInUse);
	}
	
}

void ABaseShield::RechargeLifeTime()
{
	bInUse = false;

	UE_LOG(LogTemp, Display, TEXT("CHARGING"));
	if(bOverHeat)
	{
		DelayShield = 20.f;
	}else
	{
		DelayShield = 0.f;
	}

	ShieldAction.Broadcast();
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABaseShield::ShieldLifeTimeUp, 0.1f, !bInUse, DelayShield);

}

void ABaseShield::ShieldLifeTimeDown()
{
	LifeTime -= DamageRate;
	if(LifeTime <= 0)
	{
		bOverHeat = true;
		GetWorldTimerManager().ClearTimer(UnusedHandle);

		SetShieldOff();

		DamageRate = 0.1f;
		HealthComp->ResetHealth();
	}

}

void ABaseShield::ShieldLifeTimeUp()
{
	LifeTime += 0.1f;

	if(bOverHeat && LifeTime > 3.f)
	{
		bOverHeat = false;
	}

	if(LifeTime >= MaxLifeTime)
	{
		GetWorldTimerManager().ClearTimer(UnusedHandle);
		if(HealthComp != nullptr)
		{
			HealthComp->ResetHealth();
		}
	}

}

void ABaseShield::SetShieldActive()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

}

void ABaseShield::SetShieldOff()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

}

float ABaseShield::GetBattery()
{
	return LifeTime;

}

USkeletalMeshComponent* ABaseShield::GetMesh()
{
	if(SkeletalMesh != nullptr)
	{
		return SkeletalMesh;
	}
	
	return nullptr;
	
}
