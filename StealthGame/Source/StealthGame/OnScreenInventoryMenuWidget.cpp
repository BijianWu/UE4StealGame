// Fill out your copyright notice in the Description page of Project Settings.

#include "OnScreenInventoryMenuWidget.h"
#include "Components/TextBlock.h"
#include "UIInterface.h"

void UOnScreenInventoryMenuWidget::SetupMenuInterface(IUIInterface* UIInterface)
{
	this->UIInterface = UIInterface;
}

void UOnScreenInventoryMenuWidget::Setup()
{
	this->AddToViewport();
}

bool UOnScreenInventoryMenuWidget::Initialize()
{
	bool success = Super::Initialize();
	if (!success) { return false; }
	Setup();
	return true;
}

void UOnScreenInventoryMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
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

void UOnScreenInventoryMenuWidget::ChangePillText(FString InText)
{
	PillText->SetText(FText::FromString(InText));
	//done!
}
