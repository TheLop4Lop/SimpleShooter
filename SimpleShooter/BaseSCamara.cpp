// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseSCamara.h"
#include "BasicCamaraController.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"

// Sets default values
ABaseSCamara::ABaseSCamara()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	Capsule->SetupAttachment(RootComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Camera Mesh"));
	BaseMesh->SetupAttachment(Capsule);

	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera Mesh"));
	CameraMesh->SetupAttachment(BaseMesh);

}

// Called when the game starts or when spawned
void ABaseSCamara::BeginPlay()
{
	Super::BeginPlay();

	CameraController = Cast<ABasicCamaraController>(GetController());
	if(CameraController == nullptr)
	{
		return;
	}

	PointLight = FindComponentByClass<UPointLightComponent>();
	if(PointLight != nullptr)
	{
		PointLight->Intensity = 0.f;
	}
	
}

// Called every frame
void ABaseSCamara::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CameraController->bMoveDetected && CameraController->bPlayerDetected == false)
	{	
		PointLight->SetIntensity(4000.f);
		PointLight->SetLightColor(FColor::Green);
		UE_LOG(LogTemp, Display, TEXT("Green"));
	}else if(CameraController->bMoveDetected && CameraController->bPlayerDetected)
	{
		PointLight->SetLightColor(FColor::Red);
		UE_LOG(LogTemp, Display, TEXT("Red!!"));
	}else if(CameraController->bMoveDetected == false)
	{
		PointLight->SetIntensity(0.f);
	}

}

// Called to bind functionality to input
void ABaseSCamara::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

