
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "ShooterPlayerController.h"
#include "Math/UnrealMathUtility.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "HealthActor.h"
#include "BaseShield.h"
#include "BaseAmmo.h"
#include "GunRifle.h"
#include "GunBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterController = Cast<AShooterPlayerController>(GetController());

	HealthComponent = FindComponentByClass<UHealthComponent>();
	UUserWidget* HealthWidget = CreateWidget(CharacterController, HealthClassWidget);
	if(HealthWidget != nullptr)
	{
		HealthWidget->AddToViewport();
	}

	SpawnInventory();
	
	UUserWidget* WeaponsInfo = CreateWidget(CharacterController, WeaponsInfoClass);
	if(WeaponsInfo != nullptr)
	{
		WeaponsInfo->AddToViewport();
	}

	Shield = GetWorld()->SpawnActor<ABaseShield>(ShieldClass);
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ShieldSpawn"));
	Shield->SetOwner(this);

	Shield->SetShieldOff();

	bDoOnceShieldOn = true;
	bDoOnceShieldOff = true;

}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	(InSightLine() != nullptr)? CanGrab = true : CanGrab = false;
	AddTakeObjectWidget(CanGrab);

	FallingDamage();

	if(Aiming || AimingRifle && GetCharacterMovement()->MaxWalkSpeed > 610)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}

	ShieldActivation();

}

//Character Movement
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("ZoomScopeUp"), EInputEvent::IE_Pressed, this, &AShooterCharacter::UseScopeUp);
	PlayerInputComponent->BindAction(TEXT("ZoomScopeUpDown"), EInputEvent::IE_Pressed, this, &AShooterCharacter::UseScopeDown);

	PlayerInputComponent->BindAction(TEXT("PullTrigger"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PullTrigger);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ReloadBullets);

	PlayerInputComponent->BindAction(TEXT("SetPrimaryWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SetPrimaryWeapon);
	PlayerInputComponent->BindAction(TEXT("SetSecondaryWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SetSecondaryWeapon);

	PlayerInputComponent->BindAction(TEXT("Take"), EInputEvent::IE_Pressed, this, &AShooterCharacter::TakeObject);

	PlayerInputComponent->BindAction(TEXT("LClick"), EInputEvent::IE_Pressed, this, &AShooterCharacter::StarAim);
	PlayerInputComponent->BindAction(TEXT("LClick"), EInputEvent::IE_DoubleClick, this, &AShooterCharacter::SetDoubleClick);
	PlayerInputComponent->BindAction(TEXT("LClick"), EInputEvent::IE_Released, this, &AShooterCharacter::ClickOffHandle);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AShooterCharacter::Walk);

	//Mouse
	//Instead of calling private function of AShooterCharacter, call the class parent function
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	//Controller
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookRightRate);

}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
	if(AxisValue > 0)
	{
		bMovingFoward = true;
	}else
	{
		bMovingFoward = false;
	}

}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
	
}

void AShooterCharacter::Sprint()
{
	if(bMovingFoward && Aiming == false && AimingRifle == false)
	{
		GetCharacterMovement()->MaxWalkSpeed = 900;
	}

}

void AShooterCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;

}

//Camera Movement
void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * (RotationRateY * GetWorld()->GetDeltaSeconds()));

}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * (RotationRateX * GetWorld()->GetDeltaSeconds()));

}

void AShooterCharacter::ClickOffHandle()
{
	if(Aiming || AimingRifle)
	{
		StopAim();
	}

	if(bDoubleClick)
	{
		ShieldOff();
		bDoubleClick = false;
		if(Shield->bInUse)
		{
			Shield->RechargeLifeTime();
		}
	}

}

float AShooterCharacter::GetTurnRate()
{
	float Axis = AShooterCharacter::GetInputAxisValue("LookRight");
	
	return Axis;

}

//Character Health and Damage
float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageIn = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(HealthComponent != nullptr)
	{
		HealthComponent->SetDamage(DamageIn);
		GettingDamage.Broadcast();

		if(HealthComponent->IsDead())
		{
			bDeath = true;
			ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
			if(GameMode !=nullptr)
			{
				GameMode->PawnKilled(this);
			}
			
			DetachFromControllerPendingDestroy();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	return DamageIn;
}

void AShooterCharacter::FallingDamage()
{
	if(GetMovementComponent()->IsFalling())
	{
		float Acceleration = GetMovementComponent()->Velocity.Size();
		(Acceleration > 1500)? FallDamage += (Acceleration)/1000 : FallDamage = 0;
		DoOnceFall = true;
	}

	if(GetMovementComponent()->IsFalling() != true && FallDamage > 0 && DoOnceFall)
	{
		FDamageEvent FallDamageEvent;
		TakeDamage(FallDamage, FallDamageEvent, CharacterController, this);
		DoOnceFall = false;
	}

}

UHealthComponent* AShooterCharacter::GetHealthComponent()
{
	if(HealthComponent != nullptr)
	{
		return HealthComponent;
	}

	return nullptr;

}

//Weapon Interaction
void AShooterCharacter::SpawnInventory()
{
	WeaponsInventory[0] = GetWorld()->SpawnActor<AGunBase>(Weapons[0]);
	WeaponsInventory[1] = GetWorld()->SpawnActor<AGunRifle>(Weapons[1]);

	WeaponsInventory[0]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_rSocket"));
	WeaponsInventory[0]->SetOwner(this);

	WeaponsInventory[1]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("FX_backpack"));
	WeaponsInventory[1]->SetOwner(this);

	Gun = WeaponsInventory[0];

}

void AShooterCharacter::SetPrimaryWeapon()
{
	if(Gun->Reloading)
	{
		return;
	}

	SwitchingWeapon.Broadcast();

	AimingRifle = false;
	
	FVector OriginalRifleLocation(3, -13, 19);
	FRotator OriginalRifleRotation(3, 0, 85);

	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform , TEXT("FX_backpack"));
	Gun->GetSkeletalMesh()->SetRelativeLocationAndRotation(OriginalRifleLocation, OriginalRifleRotation);

	Gun = WeaponsInventory[0];
	GetMesh()->UnHideBoneByName(TEXT("weapon_r"));

}

void AShooterCharacter::SetSecondaryWeapon()
{
	if(Gun->Reloading)
	{
		return;
	}

	SwitchingWeapon.Broadcast();

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun = WeaponsInventory[1];

	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_rSocket"));

	FVector RelativeLocation(0, -12, -3);
	FRotator NewRotation = FRotator::ZeroRotator;
	Gun->GetSkeletalMesh()->SetRelativeLocationAndRotation(RelativeLocation, NewRotation);

}

void AShooterCharacter::StarAim()
{
	Aiming = true;

	if(CharacterController != nullptr)
	{
		CharacterController->AddWidgetAim();
	}
	
}

void AShooterCharacter::StopAim()
{
	Aiming = false;
	AimingRifle = false;

	if(CharacterController != nullptr)
	{
		CharacterController->RemoveWidgetAim();
	}

	if(CharacterController != nullptr)
	{
		CharacterController->SetViewTargetWithBlend(this, 0.5f);
		ZoomIn = 100;
	}

}

void AShooterCharacter::PullTrigger()
{
	if(Gun && Aiming)
	{
		Gun->ToFire();
	}

}

void AShooterCharacter::ReloadBullets()
{
	if(Gun != nullptr)
	{
		Gun->TimerDelay();
	}
	
}

void AShooterCharacter::SetRecharge(TSubclassOf<class AGunBase>* GunType, int ChargeAmount)
{
	if(GunType != nullptr && *GunType == Weapons[0])
	{
		WeaponsInventory[0]->SetAmmoReload(&ChargeAmount);
	}else if (GunType != nullptr && *GunType == Weapons[1])
	{
		WeaponsInventory[1]->SetAmmoReload(&ChargeAmount);
	}

}

//Weapon Scope
void AShooterCharacter::UseScopeUp()
{
	AGunRifle* Rifle = Cast<AGunRifle>(Gun);

	if(Rifle != nullptr && Aiming && ZoomIn > MaxZoom + 1 )
	{
		ZoomIn = ZoomIn - MaxZoom;

		Rifle->ZoomScope(ZoomIn);
	}

}

void AShooterCharacter::UseScopeDown()
{
	AGunRifle* Rifle = Cast<AGunRifle>(Gun);

	if(Rifle != nullptr && Aiming && ZoomIn < MinZoom )
	{
		ZoomIn = ZoomIn + MaxZoom;

		Rifle->ZoomScope(ZoomIn);
	}

}

AGunBase* AShooterCharacter::GetCharacterGun()
{
	if(Gun != nullptr)
	{
		return Gun;
	}

	return nullptr;

}

int AShooterCharacter::GetBulletsToFire()
{
	if(Gun != nullptr)
	{
		return Gun->GetBulletsToFire();
	}

	return 0;

}

int AShooterCharacter::GetBateryCharge()
{
	if(Gun != nullptr)
	{
		return Gun->GetBateryCharge();
	}

	return 0;

}

//Shield Interaction
void AShooterCharacter::SetDoubleClick()
{
	bDoubleClick = true;

}

void AShooterCharacter::ShieldOn()
{
	if(ShieldClass != nullptr)
	{
		if(CharacterController != nullptr && CharacterController->bAimWidget)
		{
			CharacterController->RemoveWidgetAim();
		}

		Shield->SetShieldActive();

		Aiming = false;
		AimingRifle = false;
		bDoOnceShieldOff = true;

		Shield->ReduceLifeTime();
	}

}

void AShooterCharacter::ShieldOff()
{
	if(Shield != nullptr)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;

		Shield->SetShieldOff();

		bDoOnceShieldOn = true;
	}

}

void AShooterCharacter::ShieldActivation()
{
	if(bDoubleClick && GetCharacterMovement()->GetCurrentAcceleration().Size() == 0 
		&& bDoOnceShieldOn && Shield->bOverHeat == false)
	{
		ShieldOn();
		bDoOnceShieldOn = false;
	}else if(bDoubleClick && GetCharacterMovement()->GetCurrentAcceleration().Size() != 0 
		&& bDoOnceShieldOff && bDoOnceShieldOn == false)
	{
		ShieldOff();
		bDoOnceShieldOff = false;
	}else if(bDoubleClick == false && bDoOnceShieldOff == false)
	{
		bDoOnceShieldOff = true;
	}

}

float AShooterCharacter::GetShieldCharge()
{
	if(Shield != nullptr)
	{
		return Shield->GetBattery();
	}

	return 0;

}

ABaseShield* AShooterCharacter::GetCharacterShield()
{
	if(Shield != nullptr)
	{
		return Shield;
	}

	return nullptr;

}

//Object Interaction
AActor* AShooterCharacter::InSightLine()
{
	FVector SightLocation;
	FRotator SightRotation;
	FVector SightEndVector;

	if(CharacterController != nullptr)
	{
		CharacterController->GetPlayerViewPoint(SightLocation, SightRotation);
		SightEndVector = SightLocation + (SightRotation.Vector() * ObjectRange);
	}

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(5.f);

	if(GetWorld()->SweepSingleByChannel(Hit, SightLocation, SightEndVector, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, Sphere, Params))
	{
		return Hit.GetActor();

	}

	return nullptr;

}

void AShooterCharacter::AddTakeObjectWidget(bool bToTake)
{
	if(bToTake && DoOnce)
	{
		TakeObjectWidget = CreateWidget(CharacterController, TakeObjectClass);
		TakeObjectWidget->AddToViewport();
		DoOnce = false;
	}else if(bToTake == false && IsValid(TakeObjectWidget) && DoOnce == false && TakeObjectWidget != nullptr)
	{
		TakeObjectWidget->RemoveFromParent();
		DoOnce = true;
	}

}

void AShooterCharacter::TakeObject()
{
	if(CanGrab)
	{
		ABaseAmmo* AmmoType = Cast<ABaseAmmo>(InSightLine());
		AHealthActor* HealthPack = Cast<AHealthActor>(InSightLine());
		if(AmmoType != nullptr)
		{
			AmmoType->SetGunCharge(this);
		}
		if(HealthPack != nullptr)
		{
			HealthPack->SetCharacterHealth(this);
		}
	}

}