// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerAreaForEndpoinReplacement.h"
#include "EndPoint.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"
#include "StealthGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIPatrolCharacter.h"

// Sets default values
ATriggerAreaForEndpoinReplacement::ATriggerAreaForEndpoinReplacement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ChangceToReplaceEndpoint = 100;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	RootComponent = TriggerVolume;
}

// Called when the game starts or when spawned
void ATriggerAreaForEndpoinReplacement::BeginPlay()
{
	Super::BeginPlay();
	FString CurrentLevelName = GetWorld()->GetName();
	if (CurrentLevelName.StartsWith("S")) //"S" Stands for standard difficulty level
	{
		UE_LOG(LogTemp, Warning, TEXT("Trigger area Been Destoried"));
		this->Destroy();
	}
	ChangceToReplaceEndpoint = 100;
	CurrentEndPointIndex = 2;

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ATriggerAreaForEndpoinReplacement::OnOverlapBegin);
	if (SpawnedEndPointInstance == nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnedEndPointInstance = GetWorld()->SpawnActor<AEndPoint>(EndPointToSpawn, EndPointSpawnLocations2[0]->GetActorLocation(), EndPointSpawnLocations2[0]->GetActorRotation(), SpawnParams);
	}

	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolCharacter::StaticClass(), FoundEnemies);
	UE_LOG(LogTemp, Warning, TEXT("Trigger Area found %d enemies to assign event"), FoundEnemies.Num());
	for (auto Enemy : FoundEnemies)
	{
		AAIPatrolCharacter* AIEnemy = Cast<AAIPatrolCharacter>(Enemy);
		if (AIEnemy == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("enemy %s cannot be casted"), *AIEnemy->GetName());
		}
		AIEnemy->SetupForEndPointDelegate(this);
	}
}

// Called every frame
void ATriggerAreaForEndpoinReplacement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATriggerAreaForEndpoinReplacement::EnableGenerateEvent()
{
	GetWorldTimerManager().ClearTimer(ReEnableGenerateEventTimer);
	TriggerVolume->SetGenerateOverlapEvents(true);
	
}

void ATriggerAreaForEndpoinReplacement::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//If it is not player got detected, just return
	TArray<UActorComponent*> FoundCharacterMovementComponents;
	FoundCharacterMovementComponents = OtherActor->GetComponentsByTag(UCharacterMovementComponent::StaticClass(), FName(TEXT("PlayerC")));
	if (FoundCharacterMovementComponents.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not the player BeginOverlap"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("PLAYER Star overlap"));
	if (CurrentEndPointIndex == 2)
	{
		//Spawn in spawn location 2 By Chance
		if (SpawnedEndPointInstance == nullptr)
		{
			int32 RandomSpawnNum = FMath::RandRange(0, EndPointSpawnLocations1.Num() - 1);
			FActorSpawnParameters SpawnParams;
			SpawnedEndPointInstance = GetWorld()->SpawnActor<AEndPoint>(EndPointToSpawn, EndPointSpawnLocations1[RandomSpawnNum]->GetActorLocation(), EndPointSpawnLocations1[RandomSpawnNum]->GetActorRotation(), SpawnParams);
		}
		else
		{
			int32 ReplaceEndPointChangce = FMath::RandRange(ChangceToReplaceEndpoint-10, ChangceToReplaceEndpoint + 10);
			if (ReplaceEndPointChangce > 50)
			{
				UE_LOG(LogTemp, Warning, TEXT("Reposition endpoint from 2 to 1, changce num is %d"), ReplaceEndPointChangce);
				int32 RandomSpawnNum = FMath::RandRange(0, EndPointSpawnLocations1.Num() - 1);
				FVector NewLocation = EndPointSpawnLocations1[RandomSpawnNum]->GetActorLocation();
				SpawnedEndPointInstance->SetActorLocation(NewLocation);
				//Reduce the changce to replace the endPoint
				ChangceToReplaceEndpoint -= 30;
				UStealthGameInstance* MyGameInstance = Cast<UStealthGameInstance>(GetGameInstance());
				if (MyGameInstance != nullptr)
				{
					MyGameInstance->LoadHintMenu();
				}
				CurrentEndPointIndex = 1;
				this->SetActorLocation(NewLocation);
				TriggerVolume->SetGenerateOverlapEvents(false);
				GetWorld()->GetTimerManager().SetTimer(ReEnableGenerateEventTimer, this, &ATriggerAreaForEndpoinReplacement::EnableGenerateEvent, 1.0f, false, 1.0f);
				MYEndPointChangeDelegateSignature.Broadcast();
				return;
			}
		}
	}
	else if (CurrentEndPointIndex == 1)
	{
		//Spawn in spawn location 1 By Chance
		if (SpawnedEndPointInstance == nullptr)
		{
			int32 RandomSpawnNum = FMath::RandRange(0, EndPointSpawnLocations2.Num() - 1);
			FActorSpawnParameters SpawnParams;
			SpawnedEndPointInstance = GetWorld()->SpawnActor<AEndPoint>(EndPointToSpawn, EndPointSpawnLocations2[RandomSpawnNum]->GetActorLocation(), EndPointSpawnLocations2[RandomSpawnNum]->GetActorRotation(), SpawnParams);
		}
		else
		{
			int32 ReplaceEndPointChangce = FMath::RandRange(ChangceToReplaceEndpoint-10, ChangceToReplaceEndpoint + 10);
			if (ReplaceEndPointChangce > 50)
			{
				UE_LOG(LogTemp, Warning, TEXT("Reposition endpoint from 1 to 2, changce num is %d"), ReplaceEndPointChangce);
				int32 RandomSpawnNum = FMath::RandRange(0, EndPointSpawnLocations2.Num() - 1);
				FVector NewLocation = EndPointSpawnLocations2[RandomSpawnNum]->GetActorLocation();
				SpawnedEndPointInstance->SetActorLocation(NewLocation);
				//Reduce the changce to replace the endPoint
				ChangceToReplaceEndpoint -= 30;
				UStealthGameInstance* MyGameInstance = Cast<UStealthGameInstance>(GetGameInstance());
				if (MyGameInstance != nullptr)
				{
					MyGameInstance->LoadHintMenu();
				}
				CurrentEndPointIndex = 2;
				this->SetActorLocation(NewLocation);
				TriggerVolume->SetGenerateOverlapEvents(false);
				GetWorld()->GetTimerManager().SetTimer(ReEnableGenerateEventTimer, this, &ATriggerAreaForEndpoinReplacement::EnableGenerateEvent, 1.0f, false, 1.0f);
				MYEndPointChangeDelegateSignature.Broadcast();
				return;
			}
		}
	}


}

