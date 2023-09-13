// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseShield.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldDelegate);

UCLASS()
class SIMPLESHOOTER_API ABaseShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseShield();

	UPROPERTY(BlueprintAssignable)
	FShieldDelegate ShieldAction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	class USkeletalMeshComponent* GetMesh();

	void ReduceLifeTime();
	void RechargeLifeTime();

	void SetShieldActive();
	void SetShieldOff();
	
	float GetBattery();

	UPROPERTY(BlueprintReadOnly)
	float DelayShield;

	UPROPERTY(BlueprintReadOnly)
	bool bOverHeat;
	UPROPERTY(BlueprintReadOnly)
	bool bInUse;

private:
	UPROPERTY(EditAnywhere, Category = "Actor Properties", meta = (AllowPrivateAcces))
	USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, Category = "Actor Timelife", meta = (AllowPrivateAccess))
	float LifeTime = 20;

	UPROPERTY(EditAnywhere, Category = "Actor Timelife", meta = (AllowPrivateAccess))
	float MaxLifeTime = 20;

	float DamageRate = 0.1f;

	class USkeletalMeshComponent* SkeletalMesh;

	class UHealthComponent* HealthComp;

	FTimerHandle UnusedHandle;

	void ShieldLifeTimeUp();
	void ShieldLifeTimeDown();

};
