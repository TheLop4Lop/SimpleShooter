// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAmmo.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABaseAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetGunCharge(class AShooterCharacter* Character);

	void DestroyActor();

	TSubclassOf<class AGunBase> Weapons;

private:
	UPROPERTY(EditAnywhere, Category = "Actor Properties", meta = (AllowPrivateAccess))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Ammo Values", meta = (AllowPrivateAccess))
	float MinAmmoCharge = 0;

	UPROPERTY(EditAnywhere, Category = "Ammo Values", meta = (AllowPrivateAccess))
	float MaxAmmoCharge = 120;

	UPROPERTY(EditAnywhere, Category = "Weapon Type", meta = (AllowPrivateAccess))
	TSubclassOf<class AGunBase> TypeOfGun;

	int GetRandomCharge();

};
