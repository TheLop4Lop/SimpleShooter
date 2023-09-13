// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ShootRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/TransformNonVectorized.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BasicEnemyController.h"
#include "ShooterCharacter.h"
#include "EnemyCharacter.h"
#include "GunBase.h"

UBTService_ShootRange::UBTService_ShootRange()
{
    NodeName = TEXT("Shoot Range");

}

void UBTService_ShootRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(PlayerPawn == nullptr)
    {
        return;
    }

    AICharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if(AICharacter == nullptr)
    {
        return;
    }

    Gun = AICharacter->GetCharacterGun();
    if(Gun == nullptr)
    {
        return;
    }

    float ActualDistance = GetDistance(OwnerComp);

    if(ActualDistance <= Gun->BulletMaxDistance)
    {
        RangeToFire = true;
    }else
    {
        RangeToFire = false;
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), RangeToFire);

}

float UBTService_ShootRange::GetDistance(UBehaviorTreeComponent& OwnerComp)
{
    AAIController* Controller = OwnerComp.GetAIOwner();

    FVector Location;
    FRotator Rotation;

    Controller->GetActorEyesViewPoint(Location, Rotation);

    return FVector::Distance(Location, PlayerPawn->GetActorLocation());

}