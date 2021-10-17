// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadingScreenWidget.h"
#include "UIInterface.h"
#include "Components/TextBlock.h"
#include "Engine.h"

void ULoadingScreenWidget::SetupMenuInterface(IUIInterface * UIInterface)
{
	this->UIInterface = UIInterface;
}

bool ULoadingScreenWidget::Initialize()
{
	bool success = Super::Initialize();
	if (!success) { return false; }
	CurrentColumnsCount = 0;
	MaxAllowedColumns = 5;
	Setup();
	GetWorld()->GetTimerManager().SetTimer(LoadingTextTimer, this, &ULoadingScreenWidget::AddingColumn, .5f, true, 0.5f);
	return true;
}

void ULoadingScreenWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;

	FInputModeGameOnly inputModeData;
	playerController->SetInputMode(inputModeData);
	playerController->bShowMouseCursor = false;
	GetWorld()->GetTimerManager().ClearTimer(LoadingTextTimer);
}

void ULoadingScreenWidget::AddingColumn()
{
	if (CurrentColumnsCount > 5)
	{
		CurrentColumnsCount = 0;
		LoadingText->SetText(FText::FromString("Loading"));
	}
	else 
	{
		CurrentColumnsCount += 1;
		if (CurrentColumnsCount == 1)
		{
			LoadingText->SetText(FText::FromString("Loading."));
		}
		else if (CurrentColumnsCount == 2)
		{
			LoadingText->SetText(FText::FromString("Loading.."));
		}
		else if (CurrentColumnsCount == 3)
		{
			LoadingText->SetText(FText::FromString("Loading..."));
		}
		else if (CurrentColumnsCount == 4)
		{
			LoadingText->SetText(FText::FromString("Loading...."));
		}
		else if (CurrentColumnsCount == 5)
		{
			LoadingText->SetText(FText::FromString("Loading....."));
		}
	}
}

void ULoadingScreenWidget::Setup()
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
	playerController->bShowMouseCursor = false;
}
