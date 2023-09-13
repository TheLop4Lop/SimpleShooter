// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerDeath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasicEnemyController.h"

UBTService_PlayerDeath::UBTService_PlayerDeath()
{
    NodeName = TEXT("Player Death Status");

}

void UBTService_PlayerDeath::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if(PlayerPawn == nullptr)
    {
        return;
    }

    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return;
    }

    if(OwnerComp.GetBlackboardComponent() == nullptr)
    {
        return;
    }

    ABasicEnemyController* AIController = Cast<ABasicEnemyController>(OwnerComp.GetAIOwner());
    if(AIController == nullptr)
    {   
        return;
    }
    
    if(AIController->bPlayerDeath)
    {
        UE_LOG(LogTemp, Display, TEXT("DEEEEAAAAATHHH BIIIIIII!!!!!"));
        OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Default);
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());        
    }

}
