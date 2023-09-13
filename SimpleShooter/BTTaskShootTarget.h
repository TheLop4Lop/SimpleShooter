// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskShootTarget.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTaskShootTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskShootTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	class AEnemyCharacter* AICharacter;
	
};
