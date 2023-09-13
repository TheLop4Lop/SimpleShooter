// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskStopAim.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTaskStopAim : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskStopAim();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	class AEnemyCharacter* AICharacter;
	
};
