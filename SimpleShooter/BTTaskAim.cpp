// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskAim.h"
#include "BasicEnemyController.h"
#include "EnemyCharacter.h"

UBTTaskAim::UBTTaskAim()
{
    NodeName = TEXT("Aim Target");
}

EBTNodeResult::Type UBTTaskAim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    AICharacter->StarAim();

    return EBTNodeResult::Succeeded;
}

