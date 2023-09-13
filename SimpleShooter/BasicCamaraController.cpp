// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCamaraController.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "BasicEnemyController.h"
#include "ShooterCharacter.h"
#include "HealthComponent.h"
#include "EngineUtils.h"
#include "BaseSCamara.h"

void ABasicCamaraController::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    AICamera = Cast<ABaseSCamara>(GetPawn());

    if(PlayerCharacter == nullptr || AICamera == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player or Camera: nullptr"));

        return;
    }

    AIPerception = FindComponentByClass<UAIPerceptionComponent>();
    if(AIPerception != nullptr)
    {
        FAISenseID SenseIDSight = UAISense::GetSenseID<UAISense_Sight>();
        Sight = Cast<UAISenseConfig_Sight>(AIPerception->GetSenseConfig(SenseIDSight));

        if(Sight != nullptr)
        {
            Sight->SightRadius = 1000;
            Sight->LoseSightRadius = 1500;
            Sight->PeripheralVisionAngleDegrees = 90;
            Sight->DetectionByAffiliation.bDetectNeutrals = true;
        }

        AIPerception->ConfigureSense(*Sight);
        AIPerception->SetDominantSense(Sight->GetSenseImplementation());

        AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicCamaraController::OnTargetPerceptionUpdated);
    }

}

void ABasicCamaraController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ABasicCamaraController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    CharacterActor = Cast<AShooterCharacter>(Actor);
    if(CharacterActor != nullptr)
    {
        if(CharacterActor->ActorHasTag(TEXT("MainCharacter")))
        {
            if(Stimulus.WasSuccessfullySensed())
            {
                bMoveDetected = true;
                GetWorldTimerManager().SetTimer(DetectedTimer, this, &ABasicCamaraController::Detection, AICamera->TimeToDetect, false);
            }else
            {
                GetWorldTimerManager().ClearTimer(DetectedTimer);
                bMoveDetected = false;
                if(bPlayerDetected)
                {
                    bPlayerDetected = false;
                }
            } 
        }
    }

}

void ABasicCamaraController::Detection()
{
    bPlayerDetected = true;
    LastLocation = CharacterActor->GetActorLocation();

    UE_LOG(LogTemp, Display, TEXT("Actor Location, X: %f, Y: %f, Z: %f"), LastLocation.X, LastLocation.Y, LastLocation.Z);

    for(ABasicEnemyController* Controller: TActorRange<ABasicEnemyController>(GetWorld()))
    {
        if(Controller->EnemyCameraZone == AICamera->CameraZone)
        {
            UE_LOG(LogTemp, Display, TEXT("CALLING BACKUP"));
            Controller->bCameraPlayerDetected = true;
            Controller->CameraLocation = LastLocation;
        }
    }

}