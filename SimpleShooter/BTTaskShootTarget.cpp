// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskShootTarget.h"
#include "BasicEnemyController.h"
#include "EnemyCharacter.h"

UBTTaskShootTarget::UBTTaskShootTarget()
{
    NodeName = TEXT("Shoot Target");
}

EBTNodeResult::Type UBTTaskShootTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AICharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if(AICharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AICharacter->PullTrigger();

    return EBTNodeResult::Succeeded;
}