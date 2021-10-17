// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosionEffect.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AExplosionEffect::AExplosionEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ExplosionPS = CreateDefaultSubobject<UParticleSystemComponent>("ExplosionPS");
	RootComponent = ExplosionPS;
}

// Called when the game starts or when spawned
void AExplosionEffect::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("I am getting spawned"));
}

// Called every frame
void AExplosionEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

