// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "StealthGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "StealthGameInstance.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "BGSoundActor.h"
#include "AIPatrolCharacter.h"
//////////////////////////////////////////////////////////////////////////
// AStealthGameCharacter

AStealthGameCharacter::AStealthGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	bCanMove = true;
	MyMesh = GetMesh();
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	bHaveInvisiblePill = false;
	InvisibleTime = 3;
}

void AStealthGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	bCanMove = true;
	bIsVisible = true;
	bHaveInvisiblePill = false;
	PassedTime = 0;
	InvisibleTime = 3;
	UE_LOG(LogTemp, Warning, TEXT("setupplayer called"));
	
	//UWorld* TheWorld = GetWorld();
	//if (TheWorld != nullptr)
	//{
	//	UStealthGameInstance* MyGameInstance = Cast<UStealthGameInstance>(GetGameInstance());
	//	if (MyGameInstance)
	//	{
	//		//UE_LOG(LogTemp, Warning, TEXT("player begin play gets called"));
	//		UE_LOG(LogTemp, Warning, TEXT("current level name is %s"), *GetWorld()->GetName());
	//		MyGameInstance->CurrentPlayer = this;

	//	}
	//}
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolCharacter::StaticClass(), FoundEnemies);
	UE_LOG(LogTemp, Warning, TEXT("Player found %d enemies"), FoundEnemies.Num());
	for (auto Enemy: FoundEnemies)
	{
		AAIPatrolCharacter* AIEnemy = Cast<AAIPatrolCharacter>(Enemy);
		if (AIEnemy == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("enemy %s cannot be casted"), *AIEnemy->GetName());
		}
		AIEnemy->SetupForDelegate(this);
	}
	//Spawn BG sound background actor
	FActorSpawnParameters SpawnParams;
	SoundActorInTheLevel = GetWorld()->SpawnActor<ABGSoundActor>(SoundActorToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);

}
void AStealthGameCharacter::DisableMobility()
{
	UE_LOG(LogTemp, Warning, TEXT("disbale movility"));
	bCanMove = false;
}
//////////////////////////////////////////////////////////////////////////
// Input

void AStealthGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("ToggleVisibility", IE_Pressed, this, &AStealthGameCharacter::UsePill);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStealthGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStealthGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AStealthGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AStealthGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AStealthGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AStealthGameCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AStealthGameCharacter::OnResetVR);
}
void AStealthGameCharacter::ToggleVisibility()
{
	UE_LOG(LogTemp, Warning, TEXT("P is being pressed"));
	if (bIsVisible == true)
	{		
		bIsVisible = false;
		SetPlayerMatToOn();
		MYVisibilityChangeDelegate.Broadcast(false);
	}
	else
	{		
		bIsVisible = true;
		SetPlayerMatToOff();
		MYVisibilityChangeDelegate.Broadcast(true);
	}
}

void AStealthGameCharacter::PickupPill()
{
	auto MyGameInstance = Cast<UStealthGameInstance>(GetGameInstance());
	if (MyGameInstance)
	{
		MyGameInstance->ChangeOnScreenPillText("Press Q to use Pill To turn into invisible for 3 seconds");
		bHaveInvisiblePill = true;
	}
}

void AStealthGameCharacter::UsePill()
{
	if (bHaveInvisiblePill)
	{
		auto MyGameInstance = Cast<UStealthGameInstance>(GetGameInstance());
		if (MyGameInstance)
		{
			MyGameInstance->ChangeOnScreenPillText("Count down 0");
			ToggleVisibility();
			bHaveInvisiblePill = false;
			GetWorld()->GetTimerManager().SetTimer(InvisiblePillTimer, this, &AStealthGameCharacter::TimerStarting, 1.0f, true, 1.0f);
		}
	}
}


void AStealthGameCharacter::TimerStarting()
{
	
	auto MyGameInstance = Cast<UStealthGameInstance>(GetGameInstance());
	if (MyGameInstance)
	{
		PassedTime += 1;
		TArray< FStringFormatArg > args;
		args.Add(FStringFormatArg(PassedTime));
		FString MyString = FString::Format(TEXT("Count down {0}"), args);
		MyGameInstance->ChangeOnScreenPillText(MyString);
		
	}
	if (PassedTime >= InvisibleTime)
	{
		GetWorldTimerManager().ClearTimer(InvisiblePillTimer);
		ToggleVisibility();
		PassedTime = 0;
		MyGameInstance->ChangeOnScreenPillText("Current have no pills");
	}


}

void AStealthGameCharacter::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("BeforeBeginDestory"));
	if (SoundActorInTheLevel != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginDestory Destory sound actor"));
		SoundActorInTheLevel->Destroy();
	}
}

void AStealthGameCharacter::PlayExplosionCameraShake(AActor* EnemyActor)
{
	GetMesh()->SetHiddenInGame(true);
	if (ExplosionCameraShake != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(ExplosionCameraShake, 1.0f);
	}
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->DisableInput(GetWorld()->GetFirstPlayerController());
	FViewTargetTransitionParams TransitionParams;
	TransitionParams.BlendTime = 2.0f;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->SetViewTarget(EnemyActor, TransitionParams);
	
}

void AStealthGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AStealthGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AStealthGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AStealthGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStealthGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AStealthGameCharacter::MoveForward(float Value)
{
	
	if (!bCanMove)
	{
		
		return;
	}
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AStealthGameCharacter::MoveRight(float Value)
{
	if (!bCanMove)
	{
		
		return;
	}
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AStealthGameCharacter::SetPlayerMatToOn()
{
	if (MyMesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("My mesh is null"));
		return;
	}
	MyMesh->SetMaterial(0, OnMaterial);
}
void AStealthGameCharacter::SetPlayerMatToOff()
{
	if (MyMesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("My mesh is null"));
		return;
	}
	MyMesh->SetMaterial(0, OffMaterial);
}
