// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIPatrolController.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API AAIPatrolController : public AAIController
{
	GENERATED_BODY()
private:
	/*Our BehaviorTree Component*/
	class UBehaviorTreeComponent* BehaviorTreeComp;

	/*Our Blackboard Component*/
	class UBlackboardComponent* BlackboardComp;

	/*Blackboard keys*/
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName LocationToGoKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName Playerkey;
	virtual void Possess(APawn* Pawn) override;


public:
	TArray<AActor*> PatrolPoints;

	AAIPatrolController();

	int32 CurrentPatrolPoint;

	void SetPlayerCaught(APawn* Pawn);

	/*Inline getter functions*/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; };
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; };
};
