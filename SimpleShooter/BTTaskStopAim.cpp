// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskStopAim.h"
#include "BasicEnemyController.h"
#include "ShooterCharacter.h"
#include "EnemyCharacter.h"

UBTTaskStopAim::UBTTaskStopAim()
{
    NodeName = TEXT("Stop Aim");
}

EBTNodeResult::Type UBTTaskStopAim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    AICharacter->StopAim();

    return EBTNodeResult::Succeeded;
}

