// Fill out your copyright notice in the Description page of Project Settings.

#include "InvisiblePill.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "StealthGameCharacter.h"

// Sets default values
AInvisiblePill::AInvisiblePill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = TriggerVolume;
	MyMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInvisiblePill::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AInvisiblePill::OnOverlapBegin);
}

void AInvisiblePill::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AStealthGameCharacter* PlayerChar = Cast<AStealthGameCharacter>(OtherActor);
	if (PlayerChar != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player picked up pill"));
		PlayerChar->PickupPill();
		Destroy();
	}
}

// Called every frame
void AInvisiblePill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

