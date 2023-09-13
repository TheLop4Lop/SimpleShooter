// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectedByCamera.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasicEnemyController.h"
#include "ShooterCharacter.h"

UBTService_DetectedByCamera::UBTService_DetectedByCamera()
{
    NodeName = TEXT("Camera Detected");

}

void UBTService_DetectedByCamera::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

    if(AIController->bCameraPlayerDetected)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), AIController->CameraLocation);
    }else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }

}
