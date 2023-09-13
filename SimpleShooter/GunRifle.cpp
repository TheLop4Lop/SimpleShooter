// Fill out your copyright notice in the Description page of Project Settings.


#include "GunRifle.h"
#include "Camera/CameraComponent.h"
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"

void AGunRifle::ZoomScope(float& Zoom)
{
    ZoomCamera = FindComponentByClass<UCameraComponent>();
    AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwner());
    AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    if(ZoomCamera != nullptr && Character!= nullptr && PlayerController != nullptr)
    {   
        Character->AimingRifle = true;
        PlayerController->SetViewTargetWithBlend(this, 0.5f);
        ZoomCamera->SetFieldOfView(Zoom);
    }

}
