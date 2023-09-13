// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PatrolTP.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_PatrolTP : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PatrolTP();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	FVector TargerPoinLocation;
	class ATargetPoint* CurrentTP;
	
	int TargetPointIndex;
	
};
