//NewSimpleShooter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGettingDamageDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchingWeaponDelegate);

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	//MultiCast Variables
	UPROPERTY(BlueprintAssignable)
	FGettingDamageDelegate GettingDamage;

	UPROPERTY(BlueprintAssignable)
	FSwitchingWeaponDelegate SwitchingWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bDoOnceShieldOn;
	bool bDoOnceShieldOff;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Character Movement
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Character Health and Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UPROPERTY(BlueprintReadWrite)
	bool bGettingDamage;
	bool bDeath;

	UFUNCTION(BlueprintPure)
	class UHealthComponent* GetHealthComponent();

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> HealthClassWidget;

	//Weapons Interaction
	UPROPERTY(BlueprintReadOnly)
	bool Aiming;
	UPROPERTY(BlueprintReadOnly)
	bool AimingRifle;

	virtual void StarAim();
	virtual void StopAim();
	void PullTrigger();

	void SetRecharge(TSubclassOf<class AGunBase>* GunType, int ChargeAmount);

	UFUNCTION(BlueprintPure)
	class AGunBase* GetCharacterGun();

	UFUNCTION(BlueprintPure)
	int GetBulletsToFire();
	UFUNCTION(BlueprintPure)
	int GetBateryCharge();

	//Shield Interaction
	UPROPERTY(EditDefaultsOnly, Category = "Defense")
	TSubclassOf<class ABaseShield> ShieldClass;
	class ABaseShield* Shield;

	UFUNCTION(BlueprintPure)
	class ABaseShield* GetCharacterShield();

	void SpawnInventory();
	void FallingDamage();

	virtual void ShieldOn();
	virtual void ShieldOff();

	virtual void ShieldActivation();
	
private:
	//UPROPERTY Constructor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties", meta = (AllowPrivateAccess))
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "Character Properties", meta = (AllowPrivateAccess))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Object In Range", meta = (AllowPrivateAccess))
	float ObjectRange = 100;

	//Controllers
	class AShooterPlayerController* CharacterController;

	//Character Movement
	bool bMovingFoward;
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void Sprint();
	void Walk();

	//Camera Movement
	UPROPERTY(EditAnywhere)
	float RotationRateY = 10.f;
	UPROPERTY(EditAnywhere)
	float RotationRateX = 10.f;
	
	UFUNCTION(BlueprintCallable, Category = "Character Input", meta = (AllowPrivateAcces = "True"))
	float GetTurnRate();

	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	//Click Interactions
	FTimerHandle ClickHandle;
	void ClickOffHandle();

	//Weapons Interaction
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TArray<TSubclassOf<class AGunBase>> Weapons;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	TSubclassOf<class UUserWidget> WeaponsInfoClass;

	UPROPERTY()
	class AGunBase* Gun;
	class AGunBase* WeaponsInventory[2];

	void SetPrimaryWeapon();
	void SetSecondaryWeapon();

	void ReloadBullets();

	float MaxZoom = 10;
	float MinZoom = 100;
	float ZoomIn = 100;

	void UseScopeUp();
	void UseScopeDown();

	FTimerHandle ShieldHandle;
	bool bDoubleClick;
	
	bool HandleXtraClick;

	void SetDoubleClick();

	UFUNCTION(BlueprintPure, meta = (AllowPrivateAccess))
	float GetShieldCharge();

	//Object Interaction
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> TakeObjectClass;

	void AddTakeObjectWidget(bool bToTake);
	UUserWidget* TakeObjectWidget = nullptr;

	bool DoOnce = true;
	bool CanGrab;

	AActor* InSightLine();
	void TakeObject();	

	//Character Health and Damage
	class UHealthComponent* HealthComponent;

	float FallDamage = 0;
	bool DoOnceFall = true;

};
