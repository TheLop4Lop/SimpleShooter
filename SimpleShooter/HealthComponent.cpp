// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::SetDamage(float& Damage)
{
	Damage = FMath::Min(CurrentHealth, Damage);
	CurrentHealth -=Damage;

}

float UHealthComponent::GetHealth()
{
	return CurrentHealth/MaxHealth;
	
}

void UHealthComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;

}

bool UHealthComponent::IsDead()
{
	if(CurrentHealth == 0)
	{
		return true;
	}

	return false;

}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;

}

void UHealthComponent::SetCurrentHealth(float* Health)
{
	if(CurrentHealth += *Health > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}else
	{
		CurrentHealth += *Health;
	}

}