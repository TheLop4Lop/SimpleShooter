// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskAim.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTaskAim : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskAim();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	class AEnemyCharacter* AICharacter;
	
};
