// Copyright Epic Games, Inc. All Rights Reserved.


#include "SimpleShooterGameModeBase.h"
#include "GameFramework/Controller.h"
#include "EngineUtils.h"
#include "BasicEnemyController.h"

void ASimpleShooterGameModeBase::PawnKilled(APawn* PawnKilled)
{
    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if(PlayerController != nullptr)
    {
        EndGame(false);
    }

    for(ABasicEnemyController* Controller: TActorRange<ABasicEnemyController>(GetWorld()))
    {
        if(!Controller->IsDead())
        {
            return;
        }
    }

    EndGame(true);

}

void ASimpleShooterGameModeBase::EndGame(bool bPlayerWinner)
{
    for(AController* Controller: TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }

}