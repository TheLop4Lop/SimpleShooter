// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BasicCamaraController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ABasicCamaraController : public AAIController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bMoveDetected;
	bool bPlayerDetected;
	FVector LastLocation;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	class AShooterCharacter* PlayerCharacter;
	class ABaseSCamara* AICamera;

	class UAISenseConfig_Sight* Sight;

	UPROPERTY(EditAnywhere, Category = "AI Perception", meta = (AllowPrivateAccess))
	class UAIPerceptionComponent* AIPerception;

	class AShooterCharacter* CharacterActor;
	FTimerHandle DetectedTimer;

	void Detection();
	
};
