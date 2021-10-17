// Fill out your copyright notice in the Description page of Project Settings.

#include "AIPatrolController.h"
#include "AIPatrolCharacter.h"
#include "AIPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"


AAIPatrolController::AAIPatrolController()
{
	/*Initialize blackboard and behavior tree*/
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	/*Initialize blackboard keys*/
	Playerkey = "Target";
	LocationToGoKey = "LocationToGo";
	CurrentPatrolPoint = 0;
}

void AAIPatrolController::SetPlayerCaught(APawn * Pawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(Playerkey, Pawn);
		BehaviorTreeComp->StopLogic(TEXT("GameEnd"));
	}
}

void AAIPatrolController::Possess(APawn * Pawn)
{
	Super::Possess(Pawn);
	UE_LOG(LogTemp, Warning, TEXT("Possess got called"));
	/*Get reference to the character*/
	AAIPatrolCharacter* AICharacter = Cast<AAIPatrolCharacter>(Pawn);
	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
		}
		bool bIsFirstPatrolPointsValid = AICharacter->PatrolPoints.Num() > 0;
		bool bIsSecondPatrolPointsValid = AICharacter->PatrolPoints_2.Num() > 0;
		if (bIsFirstPatrolPointsValid && bIsSecondPatrolPointsValid)
		{
			int32 RandomNum = FMath::RandRange(0, 1);
			if (RandomNum == 0)
			{
				this->PatrolPoints = AICharacter->PatrolPoints;
			}
			else
			{
				this->PatrolPoints = AICharacter->PatrolPoints_2;
			}
		}
		else if (bIsFirstPatrolPointsValid)
		{
			this->PatrolPoints = AICharacter->PatrolPoints;
		}

		/*Populate patrol point array*/
		/*Change later to manully populate the points in editor, instead of using funtions to find*/
		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolPoint::StaticClass(), PatrolPoints);
		BehaviorTreeComp->StartTree(*AICharacter->BehaviorTree);
	}
}

