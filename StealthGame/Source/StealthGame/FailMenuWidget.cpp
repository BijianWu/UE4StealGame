// Fill out your copyright notice in the Description page of Project Settings.

#include "FailMenuWidget.h"
#include "UIInterface.h"
#include "Components/Button.h"

void UFailMenuWidget::SetupMenuInterface(IUIInterface* UIInterface)
{
	this->UIInterface = UIInterface;
}

void UFailMenuWidget::Setup()
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

bool UFailMenuWidget::Initialize()
{
	bool success = Super::Initialize();
	if (!success) { return false; }
	Setup();
	RestartButton->OnClicked.AddDynamic(this, &UFailMenuWidget::OnRestartLevelButtonClicked);
	BackToMainMenuButton->OnClicked.AddDynamic(this, &UFailMenuWidget::OnBackToMainMenuButtonClicked);
	return true;
}

void UFailMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
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

void UFailMenuWidget::OnRestartLevelButtonClicked()
{
	UIInterface->RestartLevel();
}

void UFailMenuWidget::OnBackToMainMenuButtonClicked()
{
	UIInterface->BackToMainMenu();
}
