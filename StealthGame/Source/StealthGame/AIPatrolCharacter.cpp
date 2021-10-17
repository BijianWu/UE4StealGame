// Fill out your copyright notice in the Description page of Project Settings.

#include "AIPatrolCharacter.h"
#include "AIPatrolController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "StealthGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ExplosionEffect.h"
#include "BGSoundActor.h"
#include "StealthGameCharacter.h"
#include "StealthGameInstance.h"
#include "TriggerAreaForEndpoinReplacement.h"
#include "Components/BoxComponent.h"

bool AAIPatrolCharacter::isPlayerGotKilled = false; 
// Sets default values
AAIPatrolCharacter::AAIPatrolCharacter()
{
	//Initialize senses
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(90.f);
	MySocket = CreateDefaultSubobject<UBoxComponent>(TEXT("MySocket"));
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIPatrolCharacter::OnPlayerCaught);
	}
}

void AAIPatrolCharacter::SetupForDelegate(AStealthGameCharacter *InPlayer)
{
	CurrentPlayer = InPlayer;
	UE_LOG(LogTemp, Warning, TEXT("%s have been setup delegated"), *this->GetName());
	//InPlayer->MYVisibilityChangeDelegate.BindUObject(this, &AAIPatrolCharacter::TogglePawnSensingComp);
	MyDelegateHandle = InPlayer->MYVisibilityChangeDelegate.AddUObject(this, &AAIPatrolCharacter::TogglePawnSensingComp);
	//InPlayer->MYVisibilityChangeDelegate.BindRaw(&AAIPatrolCharacter::TogglePawnSensingComp);
}

void AAIPatrolCharacter::SetupForEndPointDelegate(ATriggerAreaForEndpoinReplacement * InTriggerArea)
{
	InTriggerArea->MYEndPointChangeDelegateSignature.AddUObject(this, &AAIPatrolCharacter::ChangePatrolPath);
}

void AAIPatrolCharacter::ChangePatrolPath() 
{
	if (PatrolPoints_2.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Patrol points 2 have not been set in the current level, skip change patrol path"));
		return;
	}

	AAIPatrolController* MyController = Cast<AAIPatrolController>(this->GetController());
	if (MyController)
	{
		int32 RandomNum = FMath::RandRange(0, 1);
		if (RandomNum == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Change patrolPoints to sets 1"));
			MyController->PatrolPoints = PatrolPoints;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Change patrolPoints sets 2"));
			MyController->PatrolPoints = PatrolPoints_2;
		}
	}
}

// Called when the game starts or when spawned
void AAIPatrolCharacter::BeginPlay()
{

	Super::BeginPlay();
	AAIPatrolCharacter::isPlayerGotKilled = false;
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s does not have any patrol points been set, destroy self comman begin"), *this->GetName());
		this->Destroy();
	}

	ChangePatrolPath();

}


// Called to bind functionality to input
void AAIPatrolCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIPatrolCharacter::TogglePawnSensingComp(bool inState)
{
	if (inState == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Toggle on"));
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIPatrolCharacter::OnPlayerCaught);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Toggle off"));
		PawnSensingComp->OnSeePawn.Clear();
	}
}

void AAIPatrolCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AAIPatrolCharacter::OnPlayerCaught(APawn* Pawn)
{
	AAIPatrolController* AIController = Cast<AAIPatrolController>(GetController());

	if (AIController && !AAIPatrolCharacter::isPlayerGotKilled)
	{
		if (CurrentPlayer != nullptr)
		{
			//Empty the delegate's invocation list
			CurrentPlayer->MYVisibilityChangeDelegate.Clear();
			CurrentPlayer->GetCharacterMovement()->Velocity = FVector(0.0f);
		}

		AAIPatrolCharacter::isPlayerGotKilled = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Reloading the level...."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You have been caught!"));
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation(), 0.5f);
		AIController->SetPlayerCaught(Pawn);
		auto SkeletalMeshCompo = GetMesh();
		FVector EnemyLocation = GetActorLocation();
		FVector PlayerLocation = Pawn->GetActorLocation();
		FRotator RotationToLookAt = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, PlayerLocation);
		SetActorRotation(RotationToLookAt);
		SkeletalMeshCompo->PlayAnimation(FireAnimationToPlay, false);
		GetWorld()->GetTimerManager().SetTimer(DestoryTimer, this, &AAIPatrolCharacter::DestoryPlayerCharacter, 3.0f, false, 0.1f);
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AAIPatrolCharacter::ReloadLevel, 3.0f, false, 1.47f);

		TArray<AActor*> FoundBGSoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABGSoundActor::StaticClass(), FoundBGSoundActors);
		UE_LOG(LogTemp, Warning, TEXT("found %d BG Sound actors"), FoundBGSoundActors.Num());
		for (auto BGActor : FoundBGSoundActors)
		{
			BGActor->Destroy();
		}
	}

}
void AAIPatrolCharacter::DestoryPlayerCharacter()
{
	auto PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AExplosionEffect>(ExplosionEffectActorToSpawn, PlayerPawn->GetActorLocation(), PlayerPawn->GetActorRotation(), SpawnParams);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, PlayerPawn->GetActorLocation());
	//Player Explosion camera shake
	AStealthGameCharacter* MyCharacter = Cast<AStealthGameCharacter>(PlayerPawn);
	if (MyCharacter != nullptr)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawnAsViewTarget, MySocket->GetComponentLocation(),MySocket->GetComponentRotation());
		MyCharacter->PlayExplosionCameraShake(SpawnedActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("tring to play player explosion camera shake, but player was not found"));
	}
	//PlayerPawn->Destroy();

}
void AAIPatrolCharacter::ReloadLevel()
{
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	UStealthGameInstance* MyGameInstance = Cast<UStealthGameInstance>(GetWorld()->GetGameInstance());
	MyGameInstance->LoadFailMenu();
}


