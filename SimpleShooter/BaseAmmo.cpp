// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "ShooterCharacter.h"
#include "GunBase.h"

// Sets default values
ABaseAmmo::ABaseAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABaseAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseAmmo::SetGunCharge(AShooterCharacter* Character)
{
	if(Character != nullptr)
	{
		Character->SetRecharge(&TypeOfGun, GetRandomCharge());
	}

	DestroyActor();

}

int ABaseAmmo::GetRandomCharge()
{
	float RandNumber = FMath::FRandRange(MinAmmoCharge, MaxAmmoCharge);

	return (int)RandNumber;

}

void ABaseAmmo::DestroyActor()
{
	Destroy();

}