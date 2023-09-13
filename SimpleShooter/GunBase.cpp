// Fill out your copyright notice in the Description page of Project Settings.

#include "GunBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
//#include "ShooterCharacter.h"

// Sets default values
AGunBase::AGunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	ActualBullets = BulletsPerCharge;
	ActualBatteryCharge = BatteryCharge;
	
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunBase::ToFire()
{
	if(Reloading != true)
	{
		if(ActualBullets > 0 && BatteryFull && Reloading == false)
		{
			PullTrigger();
			if(ActualBatteryCharge == 0 && ActualBullets == 0)
			{
				BatteryFull = false;
			}
		}else
		{
			Reloading = true;
			TimerDelay();
		}
	}

}

void AGunBase::PullTrigger()
{
	if(ShotParticle != nullptr && ShotSound != nullptr)
	{
		UGameplayStatics::SpawnEmitterAttached(ShotParticle, SkeletalMesh, MuzzleName);
		UGameplayStatics::SpawnSoundAttached(ShotSound, SkeletalMesh, MuzzleName);
	}
	
	APawn* Pawn = Cast<APawn>(GetOwner());
	AController* OwnerController = Pawn->GetController();

	FHitResult Hit;
	FVector ShotDirection;
	
	bool bSuccess = GunTrace(Hit, ShotDirection, OwnerController);
	if(bSuccess && HitParticle && HitSound)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, Hit.Location, ShotDirection.Rotation());
			
		AActor* Actor = Hit.GetActor();
		if(Actor)
		{
			FPointDamageEvent DamageEvent(GetAmountOfDamage(Hit), Hit, ShotDirection, nullptr);
			Actor->TakeDamage(GetAmountOfDamage(Hit), DamageEvent, OwnerController, this);
		}
	}
	ActualBullets -= 1;
	OnFireWeapon.Broadcast();
	
}

bool AGunBase::GunTrace(FHitResult &Hit, FVector &ShotDirection, AController* OwnerController)
{	
	FVector Location;
	FRotator Rotation;
	FVector EndVector;
	
	if(OwnerController)
	{
		OwnerController->GetPlayerViewPoint(Location, Rotation);
		EndVector = Location + (Rotation.Vector() * BulletMaxDistance);
		ShotDirection = -Rotation.Vector();
		ShotAnimationDirection = ShotDirection;
	}
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, EndVector, ECollisionChannel::ECC_GameTraceChannel1, Params);

}

void AGunBase::TimerDelay()
{
	Reloading = true;
	UE_LOG(LogTemp, Display, TEXT("Reloading!"));
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGunBase::DelayReload, DelayAmount, true);

}

void AGunBase::DelayReload()
{
	Reload();
	GetWorldTimerManager().ClearTimer(UnusedHandle);

}

void AGunBase::Reload()
{
	if(BatteryFull && ActualBatteryCharge != 0)
	{	
		for (int i = ActualBullets; i < BulletsPerCharge + 1;)
		{
			ActualBullets = i++;
			ActualBatteryCharge--;

			if(ActualBullets == BulletsPerCharge)
			{
				ActualBatteryCharge++;
			}
			
			if(ActualBatteryCharge < 0)
			{
				ActualBatteryCharge = 0;
				break;
			}
		}
	}

	Reloading = false;
	
}

void AGunBase::RechargeBattery()
{
	BatteryFull = true;
	
}

void AGunBase::SetAmmoReload(int* Ammo)
{
	if(ActualBatteryCharge + *Ammo > BatteryCharge)
	{
		ActualBatteryCharge = BatteryCharge;
	}else
	{
		ActualBatteryCharge += *Ammo;
	}
	
}

//Upgrate it with FHitResult Hit for get the actor body part (if possible)
float AGunBase::GetAmountOfDamage(FHitResult Hit)
{
	FVector LocalVector = Hit.Location - GetActorLocation();
	float Distance = LocalVector.Size();

	float DamageProportion = (FMath::Sqrt(Distance * Damage))/100;
	float ProportionDamage = Damage/DamageProportion;

	//UE_LOG(LogTemp, Display, TEXT("Damage: %f"), ProportionDamage + GetAmountOfDamageByBody(Hit, ProportionDamage));
	
	return ProportionDamage + GetAmountOfDamageByBody(Hit, ProportionDamage);
}

float AGunBase::GetAmountOfDamageByBody(FHitResult& Hit, float& BaseDamageAmount)
{
	/*AShooterCharacter* HitCharacter = Cast<AShooterCharacter>(Hit.GetActor());
	if(HitCharacter != nullptr)
	{
		FVector Head = HitCharacter->GetMesh()->GetBoneLocation(TEXT("head"), EBoneSpaces::WorldSpace);
		FVector LowChest = HitCharacter->GetMesh()->GetBoneLocation(TEXT("backpack_plate_bottA_l"), EBoneSpaces::WorldSpace);
		FVector HighChest = HitCharacter->GetMesh()->GetBoneLocation(TEXT("neck_01"), EBoneSpaces::WorldSpace);
		if(Hit.Location.Z >= Head.Z)
		{
			return BaseDamageAmount * 2;
		}else if(Hit.Location.Z >= LowChest.Z && Hit.Location.Z <= HighChest.Z)
		{
			return BaseDamageAmount;
		}
	}*/
	return 0;

}

int AGunBase::GetBulletsToFire()
{
	return ActualBullets;

}

int AGunBase::GetBateryCharge()
{
	return ActualBatteryCharge;

}

USkeletalMeshComponent* AGunBase::GetSkeletalMesh()
{
	return SkeletalMesh;

}