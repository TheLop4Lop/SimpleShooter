// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationByDamage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasicEnemyController.h"
#include "ShooterCharacter.h"

UBTService_PlayerLocationByDamage::UBTService_PlayerLocationByDamage()
{
    NodeName = TEXT("Player Location Damage");

}

void UBTService_PlayerLocationByDamage::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

    if(AIController->PlayerDetectedDamage)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
        AShooterCharacter* PlayerCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if(PlayerCharacter != nullptr)
        {
            OwnerComp.GetAIOwner()->SetFocus(PlayerCharacter, EAIFocusPriority::Gameplay);
        }
    }else if(AIController->PlayerDetectedSight == false)
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
    }

}
