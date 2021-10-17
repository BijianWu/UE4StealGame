// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "UIInterface.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"

void UMainMenuWidget::SetupMenuInterface(IUIInterface * UIInterface)
{
	this->UIInterface = UIInterface;
}

bool UMainMenuWidget::Initialize()
{
	bool success = Super::Initialize();
	if (!success) { return false; }
	Setup();
	StartGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameButtonClicked);
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitGameButtonClicked);
	StandardDifficultyButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStandardDifficultyButtonClicked);
	ExpertDifficultyButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExpertDifficultyButtonClicked);
	Level1Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLevel1ButtonClicked);
	Level2Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLevel2ButtonClicked);
	Level3Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLevel3ButtonClicked);
	ChooseDifficultyBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnChooseDifficultyBackButtonClicked);
	ChooseLevelBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnChooseLevelBackButtonClicked);
	return true;
}

void UMainMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
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

void UMainMenuWidget::Setup()
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


void UMainMenuWidget::OnStartGameButtonClicked()
{
	UIInterface->PlayButtonClickSound();
	MainMenuWidgetSwitcher->SetActiveWidgetIndex(1);//Activate the Choose difficulty menu
}

void UMainMenuWidget::OnQuitGameButtonClicked()
{
	UIInterface->PlayButtonClickSound();
	UIInterface->QuitGame();
}

void UMainMenuWidget::OnStandardDifficultyButtonClicked()
{
	UIInterface->PlayButtonClickSound();
	UIInterface -> SetDifficulty("Standard");
	MainMenuWidgetSwitcher->SetActiveWidgetIndex(2);
	DifficultyLevelLabel->SetText(FText::FromString("Difficulty : Standard"));
}

void UMainMenuWidget::OnExpertDifficultyButtonClicked()
{
	UIInterface->PlayButtonClickSound();
	UIInterface->SetDifficulty("Expert");
	MainMenuWidgetSwitcher->SetActiveWidgetIndex(2);
	DifficultyLevelLabel->SetText(FText::FromString("Difficulty : Expert"));
}

void UMainMenuWidget::OnLevel1ButtonClicked()
{
	UIInterface->StartGame("Level1");
}

void UMainMenuWidget::OnLevel2ButtonClicked()
{
	UIInterface->StartGame("Level2");
}

void UMainMenuWidget::OnLevel3ButtonClicked()
{
	UIInterface->StartGame("Level3");
}

void UMainMenuWidget::OnChooseDifficultyBackButtonClicked()
{
	UIInterface->PlayButtonClickSound();
	MainMenuWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenuWidget::OnChooseLevelBackButtonClicked()
{
	UIInterface->PlayButtonClickSound();
	MainMenuWidgetSwitcher->SetActiveWidgetIndex(1);
}
