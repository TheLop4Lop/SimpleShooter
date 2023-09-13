// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "GunRifle.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AGunRifle : public AGunBase
{
	GENERATED_BODY()

public:
	void ZoomScope(float& Zoom);

private:
	class UCameraComponent* ZoomCamera;
	
};
