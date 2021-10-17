// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSelectPatrolPoint.h"
#include "AIPatrolPoint.h"
#include "AIPatrolController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTSelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIPatrolController* AIController = Cast<AAIPatrolController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		/*Get Blackboard Component*/
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComp();

		AAIPatrolPoint* CurrentPoint = Cast<AAIPatrolPoint>(BlackboardComp->GetValueAsObject("LocationToGo"));
		TArray<AActor*> AvailablePatrolPoints = AIController->GetPatrolPoints();

		AAIPatrolPoint* NextPatrolPoint = nullptr;

		if (AIController->CurrentPatrolPoint != (AvailablePatrolPoints.Num() - 1))
		{
			NextPatrolPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoints[++AIController->CurrentPatrolPoint]);
		}
		else //if there is no more points to go to, means at the last patrol point
		{	//set patrol point to be the first patrol point
			NextPatrolPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoints[0]);

			AIController->CurrentPatrolPoint = 0; 
		}

		BlackboardComp->SetValueAsObject("LocationToGo", NextPatrolPoint);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
