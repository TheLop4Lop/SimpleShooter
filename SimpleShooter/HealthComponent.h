// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetDamage(float& Damage);

	UFUNCTION(BlueprintPure)
	bool IsDead();

	UFUNCTION(BlueprintPure)
	float GetHealth();

	UFUNCTION(BlueprintPure)
	float GetMaxHealth();
	void ResetHealth();

	void SetCurrentHealth(float* Health);

private:
	UPROPERTY(EditAnywhere, Category = "Actor Health", meta = (AllowPrivateAccess))
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Health", meta = (AllowPrivateAccess))
	float CurrentHealth;
		
};
