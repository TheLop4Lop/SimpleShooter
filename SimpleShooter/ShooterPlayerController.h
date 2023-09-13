// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	bool bAimWidget;
	void AddWidgetAim();

	void RemoveWidgetAim();

private:
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoseWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AimClassWidget;

	UUserWidget* AimWidget;

	FTimerHandle TimerRestart;

};
