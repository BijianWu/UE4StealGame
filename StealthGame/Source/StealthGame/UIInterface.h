// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STEALTHGAME_API IUIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartGame(FString LevelName) = 0;
	virtual void QuitGame() = 0;
	virtual void LoadMainMenu() = 0;
	virtual void LoadInGameMenu() = 0;
	virtual void CancelMenu() = 0;
	virtual void BackToMainMenu() = 0;
	virtual void RestartLevel() = 0;
	virtual void LoadNextLevel() = 0;
	virtual void LoadHintMenu() = 0;
	virtual void OnStandardDifficultyButtonClicked(FString NextLevelName) = 0;
	virtual void OnExpertDifficultyButtonClicked(FString NextLevelName) = 0;
	virtual void ShowCompletedLevelMenu() = 0;
	virtual void SetDifficulty(FString Difficulty) = 0;
	virtual void LoadLoadingScreen() = 0;
	virtual void PlayButtonClickSound() = 0;
};
