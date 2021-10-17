// Fill out your copyright notice in the Description page of Project Settings.

#include "EndPoint.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "StealthGameInstance.h"
#include "AIPatrolCharacter.h"
#include "BGSoundActor.h"
#include "StealthGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEndPoint::AEndPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	EndPointPS = CreateDefaultSubobject< UParticleSystemComponent>(FName("EndPointPS"));
	RootComponent = TriggerVolume;
	EndPointPS->AttachTo(RootComponent);
	
}

// Called when the game starts or when spawned
void AEndPoint::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEndPoint::OnOverlapBegin);

}

// Called every frame
void AEndPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndPoint::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//If it is not player got detected, just return
	TArray<UActorComponent*> FoundCharacterMovementComponents;
	FoundCharacterMovementComponents = OtherActor->GetComponentsByTag(UCharacterMovementComponent::StaticClass(), FName(TEXT("PlayerC")));
	if (FoundCharacterMovementComponents.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not the player BeginOverlap"));
		return;
	}

	AStealthGameCharacter* PlayerChar = Cast<AStealthGameCharacter>(OtherActor);
	if (PlayerChar != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Make player velocity equal to 0"));
		PlayerChar->Destroy();
	}
	UE_LOG(LogTemp, Warning, TEXT("Overlapped actor is %s"), *OtherActor->GetName());
	UStealthGameInstance* MyGameInstance = Cast<UStealthGameInstance>(GetWorld()->GetGameInstance());
	TArray<AActor*> FoundAIEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolCharacter::StaticClass(), FoundAIEnemies);
	UE_LOG(LogTemp, Warning, TEXT("found %d enemies"), FoundAIEnemies.Num());
	for (auto AIEnemy : FoundAIEnemies)
	{
		AIEnemy->Destroy();
	}

	TArray<AActor*> FoundBGSoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABGSoundActor::StaticClass(), FoundBGSoundActors);
	UE_LOG(LogTemp, Warning, TEXT("found %d BG Sound actors"), FoundBGSoundActors.Num());
	for (auto BGActor : FoundBGSoundActors)
	{
		BGActor->Destroy();
	}

	MyGameInstance->MYStandardDelegate.ExecuteIfBound();
	MyGameInstance->LoadSuccessMenu();
}

