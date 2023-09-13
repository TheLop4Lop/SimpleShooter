// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolTP.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BasicEnemyController.h"
#include "Engine/TargetPoint.h"
#include "EnemyCharacter.h"

UBTTask_PatrolTP::UBTTask_PatrolTP()
{
    NodeName = TEXT("Target Point Patrol");

}

EBTNodeResult::Type UBTTask_PatrolTP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ABasicEnemyController* AIController = Cast<ABasicEnemyController>(OwnerComp.GetAIOwner());
    if(AIController == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if(AICharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    if(TargetPointIndex < AICharacter->FollowPath.Num()-1)
    {
        TargetPointIndex++;
    }else if(TargetPointIndex == AICharacter->FollowPath.Num()-1)
    {
        TargetPointIndex = 0;
    }

    CurrentTP = AICharacter->FollowPath[TargetPointIndex];
    TargerPoinLocation = CurrentTP->GetActorLocation();

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey() ,TargerPoinLocation);

    return EBTNodeResult::Succeeded;

}