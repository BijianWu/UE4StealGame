// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIPatrolCharacter.generated.h"

UCLASS()
class STEALTHGAME_API AAIPatrolCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIPatrolCharacter();
	void SetupForDelegate(class AStealthGameCharacter* InPlayer);
	void SetupForEndPointDelegate(class ATriggerAreaForEndpoinReplacement* InTriggerArea);

	void ChangePatrolPath();

	class AStealthGameCharacter* CurrentPlayer;
	UPROPERTY(EditAnywhere, Category = SocketActor)
		TSubclassOf<class AActor> ActorToSpawnAsViewTarget;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	static bool isPlayerGotKilled;
public:	
	UPROPERTY(EditAnywhere, Category = AI)
	TArray<AActor*> PatrolPoints;
	UPROPERTY(EditAnywhere, Category = AI)
		TArray<AActor*> PatrolPoints_2;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//The BehaviorTree that going to be set inside Blueprint derived class 
	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* BehaviorTree;

	//The Component acts as Sensers to see if players are seen within defined detection range
	UPROPERTY(VisibleAnywhere, Category = AI)
	class UPawnSensingComponent* PawnSensingComp;
	UPROPERTY(EditAnywhere, Category = Sounds)
		class USoundBase* ShootSound;
	UPROPERTY(EditAnywhere, Category = Sounds)
		class USoundBase* ExplosionSound;

	UFUNCTION()
	void TogglePawnSensingComp(bool inState);
	FDelegateHandle MyDelegateHandle;
	virtual void PostInitializeComponents() override;
private:
	UFUNCTION()
	void OnPlayerCaught(APawn* Pawn);
	void ReloadLevel();
	void DestoryPlayerCharacter();
	FTimerHandle ReloadTimer;
	FTimerHandle DestoryTimer;
	UPROPERTY(EditAnywhere, Category = AI)
	class UAnimationAsset* FireAnimationToPlay;

	UPROPERTY(EditAnywhere, Category = EffectActor)
	TSubclassOf<class AExplosionEffect> ExplosionEffectActorToSpawn;
	UPROPERTY(VisibleAnywhere, Category = Socket)
	class UBoxComponent* MySocket;
};
