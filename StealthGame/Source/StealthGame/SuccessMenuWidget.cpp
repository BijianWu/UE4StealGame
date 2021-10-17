// Fill out your copyright notice in the Description page of Project Settings.

#include "SuccessMenuWidget.h"
#include "UIInterface.h"
#include "Components/Button.h"

void USuccessMenuWidget::SetupMenuInterface(IUIInterface* UIInterface)
{
	this->UIInterface = UIInterface;
}

void USuccessMenuWidget::Setup()
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

bool USuccessMenuWidget::Initialize()
{
	bool success = Super::Initialize();
	if (!success) { return false; }
	Setup();
	LoadNextLevelButton->OnClicked.AddDynamic(this, &USuccessMenuWidget::OnLoadNextLevelButtonClicked);
	BackToMainMenuButton->OnClicked.AddDynamic(this, &USuccessMenuWidget::OnBackToMainMenuButtonClicked);
	return true;
}

void USuccessMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
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

void USuccessMenuWidget::OnLoadNextLevelButtonClicked()
{
	UIInterface->LoadNextLevel();
}

void USuccessMenuWidget::OnBackToMainMenuButtonClicked()
{
	UIInterface->BackToMainMenu();
}
