// Fill out your copyright notice in the Description page of Project Settings.

#include "LastLevelMenuWidget.h"
#include "UIInterface.h"
#include "Components/Button.h"

void ULastLevelMenuWidget::SetupMenuInterface(IUIInterface* UIInterface)
{
	this->UIInterface = UIInterface;
}

void ULastLevelMenuWidget::Setup()
{
	this->AddToViewport();
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;

	FInputModeUIOnly inputModeData;
	inputModeData.SetWidgetToFocus(this->TakeWidget());
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	playerController->SetInputMode(inputModeData);
	playerController->bShowMouseCursor = true;
}

bool ULastLevelMenuWidget::Initialize()
{
	bool success = Super::Initialize();
	if (!success) { return false; }
	Setup();
	BackToMainMenuButton->OnClicked.AddDynamic(this, &ULastLevelMenuWidget::OnBackToMainMenuButtonClicked);
	return true;
}

void ULastLevelMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
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

void ULastLevelMenuWidget::OnBackToMainMenuButtonClicked()
{
	UIInterface->BackToMainMenu();
}




