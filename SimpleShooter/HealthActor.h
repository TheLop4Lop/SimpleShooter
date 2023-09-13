// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthActor.generated.h"

UCLASS()
class SIMPLESHOOTER_API AHealthActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCharacterHealth(class AShooterCharacter* PlayerCharacter);

private:
	UPROPERTY(EditAnywhere, Category = "Actor Properties", meta = (AllowPrivateAccess))
	UStaticMeshComponent* StaticMesh;

	void HandleActorDestroy();

};
