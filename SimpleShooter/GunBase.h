// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShotDelegate);

UCLASS()
class SIMPLESHOOTER_API AGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBase();

	UPROPERTY(BlueprintAssignable)
	FOnShotDelegate OnFireWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Actor Attack", meta = (AllowPrivateAccess))
	float BulletMaxDistance = 3000;

	UPROPERTY(BlueprintReadWrite, Category = "Actor DamageAnimation", meta = (AllowPrivateAccess))
	FVector ShotAnimationDirection;

	bool Reloading;

	void SetAmmoReload(int* Ammo);

	void RechargeBattery();

	int GetBulletsToFire();
	int GetBateryCharge();
	class USkeletalMeshComponent* GetSkeletalMesh();

	void TimerDelay();

	void ToFire();
	void PullTrigger();

private:
	UPROPERTY(EditAnywhere, Category = "Actor Properties", meta = (AllowPrivateAcces))
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = "Actor Properties", meta = (AllowPrivateAcces))
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Actor Effects", meta = (AllowPrivateAccess))
	class UParticleSystem* ShotParticle;

	UPROPERTY(EditAnywhere, Category = "Actor Effects", meta = (AllowPrivateAccess))
	USoundBase* ShotSound;

	UPROPERTY(EditAnywhere, Category = "Actor Effects", meta = (AllowPrivateAccess))
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Actor Effects", meta = (AllowPrivateAccess))
	class UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, Category = "Actor Ammo", meta = (AllowPrivateAccess))
	float Damage = 25;

	UPROPERTY(EditAnywhere, Category = "Actor Ammo", meta = (AllowPrivateAccess))
	int BulletsPerCharge = 30;

	UPROPERTY(EditAnywhere, Category = "Actor Ammo", meta = (AllowPrivateAccess))
	int BatteryCharge = 120;

	UPROPERTY(EditAnywhere, Category = "Actor Ammo", meta = (AllowPrivateAccess))
	bool BatteryFull = true;

	UPROPERTY(EditAnywhere, Category = "Reload Delay", meta = (AllowPrivateAccess))
	int DelayAmount = 3;

	UPROPERTY(EditAnywhere, Category = "Muzzle Name", Meta = (AllowPrivateAccess))
	FName MuzzleName = "RifleMagSocket";

	int ActualBullets;
	int ActualBatteryCharge;

	FTimerHandle UnusedHandle;

	void Reload();

	void DelayReload();

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection, AController* OwnerController);

	float GetAmountOfDamage(FHitResult Hit);
	float GetAmountOfDamageByBody(FHitResult& Hit, float& BaseDamageAmount);

};
