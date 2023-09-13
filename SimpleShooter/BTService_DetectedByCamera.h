// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_DetectedByCamera.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTService_DetectedByCamera : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_DetectedByCamera();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	APawn* PlayerPawn;
	
};
