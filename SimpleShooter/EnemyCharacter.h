// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AEnemyCharacter : public AShooterCharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    virtual void StarAim() override;
	virtual void StopAim() override;

    //AIController CameraZone
	UPROPERTY(EditAnywhere, Category = "Camera Zone")
	FName CharacterCameraZone;

	UPROPERTY(EditAnywhere, Category = "AICharacter Path")
	TArray<class ATargetPoint*> FollowPath;

	bool bShieldOrder;

private:
    class ABasicEnemyController* AIController;

	//Character Health and Damage
	class UHealthComponent* HealthComponent;

    virtual void ShieldOn() override;
    virtual void ShieldOff() override;

    virtual void ShieldActivation() override;

};
