// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BasicEnemyController.generated.h"


/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ABasicEnemyController : public AAIController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsDead() const;

	UPROPERTY(BlueprintReadWrite)
	bool PlayerDetectedSight;

	UPROPERTY(BlueprintReadWrite)
	bool PlayerDetectedDamage;
	
	UPROPERTY(BlueprintReadWrite)
	bool bPlayerDeath;

	bool DoOnce;

	bool bCameraPlayerDetected;
	FVector CameraLocation;

	FName EnemyCameraZone;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	class AShooterCharacter* PlayerCharacter;
	class AEnemyCharacter* AICharacter;

	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowAccess))
	class UBehaviorTree* AIBehavior;

	class UAISenseConfig_Sight* Sight;

	UPROPERTY(EditAnywhere, Category = "AI Perception", meta = (AllowPrivateAccess))
	class UAIPerceptionComponent* AIPerception;

	FTimerHandle TimerDelay;
	void DetectedByDamageTimer();
	
};
