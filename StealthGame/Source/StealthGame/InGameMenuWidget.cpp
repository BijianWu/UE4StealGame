// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenuWidget.h"
#include "UIInterface.h"
#include "Components/Button.h"

void UInGameMenuWidget::SetupMenuInterface(IUIInterface* UIInterface)
{
	this->UIInterface = UIInterface;
}

void UInGameMenuWidget::Setup()
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

bool UInGameMenuWidget::Initialize()
{
	bool success = Super::Initialize();
	if (!success) { return false; }
	Setup();
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnCancelButtonClicked);
	RestartLevelButton->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnRestartLevelButtonClicked);
	BackToMainMenuButton->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnBackToMainMenuButtonClicked);
	return true;
}

void UInGameMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
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

void UInGameMenuWidget::OnCancelButtonClicked()
{
	UIInterface->PlayButtonClickSound();
	this->RemoveFromViewport();
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;

	FInputModeGameOnly inputModeData;
	playerController->SetInputMode(inputModeData);
	playerController->bShowMouseCursor = false;
}

void UInGameMenuWidget::OnRestartLevelButtonClicked()
{
	UIInterface->RestartLevel();
}

void UInGameMenuWidget::OnBackToMainMenuButtonClicked()
{
	UIInterface->BackToMainMenu();
}

