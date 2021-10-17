// Fill out your copyright notice in the Description page of Project Settings.

#include "BGSoundActor.h"
#include "Components/AudioComponent.h"
#include "Engine.h"

// Sets default values
ABGSoundActor::ABGSoundActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyAudioComponent = CreateDefaultSubobject< UAudioComponent>(TEXT("MyAudioComponent"));
	MyAudioComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ABGSoundActor::BeginPlay()
{
	Super::BeginPlay();
	MyAudioComponent->SetSound(BGSound);
	MyAudioComponent->Play();
}

// Called every frame
void ABGSoundActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

