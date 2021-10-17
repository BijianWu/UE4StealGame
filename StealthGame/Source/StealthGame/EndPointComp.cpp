// Fill out your copyright notice in the Description page of Project Settings.

#include "EndPointComp.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UEndPointComp::UEndPointComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEndPointComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEndPointComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	const float TimeBetweenCameraChanges = 2.0f;
	const float SmoothBlendTime = 0.75f;


	TimeToNextCameraChange -= DeltaTime;

	if (TimeToNextCameraChange <= 0.0f)
	{
		TimeToNextCameraChange += TimeBetweenCameraChanges;

		APlayerController* OurPlayer = UGameplayStatics::GetPlayerController(this,0);

		if (OurPlayer)
		{
			if (CameraTwo && OurPlayer->GetViewTarget() == CameraTwo)
			{
				OurPlayer->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
			}
			else if(CameraOne)
			{
				OurPlayer->SetViewTarget(CameraOne);
			}
		}
	}

}

