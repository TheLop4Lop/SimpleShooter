// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ShootRange.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTService_ShootRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_ShootRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AEnemyCharacter* AICharacter;
	APawn* PlayerPawn;
	class AGunBase* Gun;

	bool RangeToFire;

	float GetDistance(UBehaviorTreeComponent& OwnerComp);
	
};
