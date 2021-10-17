// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIInterface.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupMenuInterface(IUIInterface* UIInterface);
	void Setup();
protected:
	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
private:
	IUIInterface * UIInterface;
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* StandardDifficultyButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExpertDifficultyButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MainMenuWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UButton* Level1Button;

	UPROPERTY(meta = (BindWidget))
		class UButton* Level2Button;

	UPROPERTY(meta = (BindWidget))
		class UButton* Level3Button;
	UPROPERTY(meta = (BindWidget))
		class UButton*ChooseDifficultyBackButton;

	UPROPERTY(meta = (BindWidget))
		class UButton*ChooseLevelBackButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyLevelLabel;

	UFUNCTION()
		void OnStartGameButtonClicked();

	UFUNCTION()
		void OnQuitGameButtonClicked();

	UFUNCTION()
		void OnStandardDifficultyButtonClicked();

	UFUNCTION()
		void OnExpertDifficultyButtonClicked();

	UFUNCTION()
		void OnLevel1ButtonClicked();

	UFUNCTION()
		void OnLevel2ButtonClicked();

	UFUNCTION()
		void OnLevel3ButtonClicked();
	UFUNCTION()
		void OnChooseDifficultyBackButtonClicked();
	UFUNCTION()
		void OnChooseLevelBackButtonClicked();
};
