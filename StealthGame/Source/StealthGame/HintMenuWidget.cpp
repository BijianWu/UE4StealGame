// Fill out your copyright notice in the Description page of Project Settings.

#include "HintMenuWidget.h"
#include "UIInterface.h"
#include "Components/Button.h"
#include "Engine.h"

void UHintMenuWidget::SetupMenuInterface(IUIInterface* UIInterface)
{
	this->UIInterface = UIInterface;
}

void UHintMenuWidget::Setup()
{
	this->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(DisappearTimer, this, &UHintMenuWidget::MakeMenuDisappear, 3.0f, false, 2.0f);
}

bool UHintMenuWidget::Initialize()
{
	bool success = Super::Initialize();
	if (!success) { return false; }
	Setup();

	return true;
}

void UHintMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;

	FInputModeGameOnly inputModeData;
	playerController->SetInputMode(inputModeData);
	playerController->bShowMouseCursor = false;
}

void UHintMenuWidget::MakeMenuDisappear()
{
	this->RemoveFromViewport();
}





