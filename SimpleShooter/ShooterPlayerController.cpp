// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

   if(bIsWinner)
   {
        UUserWidget* WinClassWidget = CreateWidget(this, WinWidget);
        if(WinClassWidget != nullptr)
        {
            WinClassWidget->AddToViewport();
        }
   }else
   {
        UUserWidget* LoseClassWidget = CreateWidget(this, LoseWidget);
        if(LoseClassWidget != nullptr)
        {
            LoseClassWidget->AddToViewport();
        }
   }

    GetWorldTimerManager().SetTimer(TimerRestart, this, &APlayerController::RestartLevel, RestartDelay);

}

void AShooterPlayerController::AddWidgetAim()
{
    AimWidget = CreateWidget(this, AimClassWidget);

    if(AimWidget != nullptr)
    {
        bAimWidget = true;
        AimWidget->AddToViewport();
    }
   
}

void AShooterPlayerController::RemoveWidgetAim()
{
    if(AimWidget != nullptr)
    {
        bAimWidget = false;
        AimWidget->RemoveFromParent();
    }
}
