// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthActor.h"
#include "ShooterCharacter.h"
#include "HealthComponent.h"

// Sets default values
AHealthActor::AHealthActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHealthActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthActor::SetCharacterHealth(AShooterCharacter* PlayerCharacter)
{
	if(PlayerCharacter != nullptr)
	{
		float MaxHealth = PlayerCharacter->GetHealthComponent()->GetMaxHealth();
		PlayerCharacter->GetHealthComponent()->SetCurrentHealth(&MaxHealth);
	}

	HandleActorDestroy();

}

void AHealthActor::HandleActorDestroy()
{
	Destroy();

}