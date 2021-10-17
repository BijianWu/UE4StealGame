// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerAreaForEndpoinReplacement.generated.h"

DECLARE_MULTICAST_DELEGATE(FEndPointChangeDelegateSignature)
UCLASS()
class STEALTHGAME_API ATriggerAreaForEndpoinReplacement : public AActor
{
	GENERATED_BODY()

	class AEndPoint* SpawnedEndPointInstance;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerVolume;
	int32 ChangceToReplaceEndpoint;
	int32 CurrentEndPointIndex;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle ReEnableGenerateEventTimer;
	void EnableGenerateEvent();
public:	
	// Sets default values for this actor's properties
	ATriggerAreaForEndpoinReplacement();
	FEndPointChangeDelegateSignature MYEndPointChangeDelegateSignature;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = EndPoint)
	TArray<AActor*> EndPointSpawnLocations1;
	UPROPERTY(EditAnywhere, Category = EndPoint)
	TArray<AActor*> EndPointSpawnLocations2;
	UPROPERTY(EditAnywhere, Category = EndPoint)
	TSubclassOf<class AEndPoint> EndPointToSpawn;

};
