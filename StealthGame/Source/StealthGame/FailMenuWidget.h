// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIInterface.h"
#include "FailMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UFailMenuWidget : public UUserWidget
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
		class UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackToMainMenuButton;

	UFUNCTION()
		void OnRestartLevelButtonClicked();

	UFUNCTION()
		void OnBackToMainMenuButtonClicked();

	
	
};
