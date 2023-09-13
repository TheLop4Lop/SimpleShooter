// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemyController.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "HealthComponent.h"
#include "EnemyCharacter.h"

void ABasicEnemyController::BeginPlay()
{
    Super::BeginPlay();

    AICharacter = Cast<AEnemyCharacter>(GetPawn());

    PlayerCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if(AIBehavior && PlayerCharacter)
    {
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());        
    }

    AIPerception = FindComponentByClass<UAIPerceptionComponent>();
    if(AIPerception != nullptr)
    {
        FAISenseID SenseIDSight = UAISense::GetSenseID<UAISense_Sight>();
        Sight = Cast<UAISenseConfig_Sight>(AIPerception->GetSenseConfig(SenseIDSight));
        if(Sight != nullptr)
        {
            Sight->SightRadius = 3500.f;
            Sight->LoseSightRadius = 4500.f;
            Sight->DetectionByAffiliation.bDetectNeutrals = true;
        }

        AIPerception->ConfigureSense(*Sight);
        AIPerception->SetDominantSense(Sight->GetSenseImplementation());

        AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicEnemyController::OnTargetPerceptionUpdated);
    }

}

void ABasicEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(PlayerDetectedSight && PlayerDetectedDamage)
    {
        PlayerDetectedDamage = false;
    }else if(PlayerDetectedDamage && !PlayerDetectedSight && DoOnce)
    {
        UE_LOG(LogTemp, Display, TEXT("DETECTEDPlayer"));
        GetWorldTimerManager().SetTimer(TimerDelay, this, &ABasicEnemyController::DetectedByDamageTimer, 5.f, false);
        DoOnce = false;
    }

    if(!PlayerDetectedSight && !PlayerDetectedDamage)
    {
        AICharacter->GetCharacterMovement()->MaxWalkSpeed = 500.f;
    }else
    {
        AICharacter->GetCharacterMovement()->MaxWalkSpeed = 650.f;
    }

}

void ABasicEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    AShooterCharacter* CharacterActor = Cast<AShooterCharacter>(Actor);
    if(CharacterActor != nullptr)
    {
        if(CharacterActor->ActorHasTag(TEXT("MainCharacter")))
        {
            if(Stimulus.WasSuccessfullySensed())
            {
                PlayerDetectedSight = true;
            }else
            {
                PlayerDetectedSight = false;
            } 
            SetFocus(Stimulus.WasSuccessfullySensed()? CharacterActor : nullptr);
        }
    }

}

bool ABasicEnemyController::IsDead() const
{
    UHealthComponent* AIHealthComponent = AICharacter->GetHealthComponent();

    if(AIHealthComponent != nullptr)
    {
        return AIHealthComponent->IsDead();
    }

    return true;
    
}

void ABasicEnemyController::DetectedByDamageTimer()
{   
    PlayerDetectedDamage = false;

}