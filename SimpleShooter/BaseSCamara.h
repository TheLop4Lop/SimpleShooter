// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseSCamara.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABaseSCamara : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseSCamara();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Camera Zone")
	FName CameraZone;

	UPROPERTY(EditAnywhere, Category = "Camera Detection")
	float TimeToDetect;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "Pawn Properties", meta = (AllowPrivateAccess))
	class UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, Category = "Pawn Properties", meta = (AllowPrivateAccess))
	UStaticMeshComponent* CameraMesh;

	UPROPERTY(EditAnywhere, Category = "Pawn Properties", meta = (AllowPrivateAccess))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Camera Properties", meta = (AllowPrivateAccess))
	class UPointLightComponent* PointLight;

	FRotator CameraAngle;
	bool bScaned;

	class ABasicCamaraController* CameraController;

};
