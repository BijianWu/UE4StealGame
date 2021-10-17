// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UIInterface.h"
#include "StealthGameInstance.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FStandardDelegateSignature)
UCLASS()
class STEALTHGAME_API UStealthGameInstance : public UGameInstance, public IUIInterface
{
	GENERATED_BODY()

public:
	UStealthGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;
	UFUNCTION(BlueprintCallable)
	void StartGame(FString LevelName);
	UFUNCTION(BlueprintCallable)
	void QuitGame();
	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();
	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();
	UFUNCTION(BlueprintCallable)
	void CancelMenu();
	UFUNCTION(BlueprintCallable)
	void BackToMainMenu();
	UFUNCTION(BlueprintCallable)
	void RestartLevel();
	UFUNCTION(BlueprintCallable)
	void LoadNextLevel();
	UFUNCTION(BlueprintCallable)
	void LoadFailMenu();
	UFUNCTION(BlueprintCallable)
	void LoadSuccessMenu();
	UFUNCTION(BlueprintCallable)
	void LoadHintMenu();
	UFUNCTION(BlueprintCallable)
	void LoadOnScreenMenu();
	UFUNCTION(BlueprintCallable)
	void OnStandardDifficultyButtonClicked(FString NextLevelName);
	UFUNCTION(BlueprintCallable)
	void OnExpertDifficultyButtonClicked(FString NextLevelName);

	FStandardDelegateSignature MYStandardDelegate;
	UFUNCTION(BlueprintCallable)
	void ChangeOnScreenPillText(FString InText);

	UFUNCTION(BlueprintCallable)
	void ShowCompletedLevelMenu();
	UFUNCTION(BlueprintCallable)
	void SetDifficulty(FString Difficulty);

	UFUNCTION(BlueprintCallable)
		void LoadLoadingScreen();
	UFUNCTION(BlueprintCallable)
	void PlayButtonClickSound();
	FString Difficulty;


	UFUNCTION(Exec)
		void CheatCode(int32 InCode);
private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	TSubclassOf<class UUserWidget> FailMenuClass;
	TSubclassOf<class UUserWidget> SuccessMenuClass;
	TSubclassOf<class UUserWidget> LastLevelMenuClass;
	TSubclassOf<class UUserWidget> PopupHintMenuClass;
	TSubclassOf<class UUserWidget> OnScreenMenuClass;
	TSubclassOf<class UUserWidget> LoadingScreenMenuClass;

	UPROPERTY()
	USoundBase* ButtonClickSound;
	USoundBase* WinSound;
	USoundBase* LoseSound;
	FTimerHandle DelayStartGameHandle;
	FTimerHandle DelayQuitGameHandle;
	FTimerHandle DelayBackToMainMenuHandle;
	FTimerHandle DelayRestartLevelMenuHandle;
	FTimerHandle DelayLoadNextLevelMenuHandle;
	UPROPERTY()
	class UOnScreenInventoryMenuWidget* PopupHintMenuInstance;
	void LoadTheGame( );

	void Level1Completed();
	void Level2Completed();

	void OpenTheLevel();

	FName LevelToOpen;
	void QuitTheGame();
	void RestartTheLevel();
	void BackToTheMainMenu();
	void LoadTheNextLevel();

};
