// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTServiceLocation.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTServiceLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTServiceLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	APawn* PlayerPawn;
	
};
