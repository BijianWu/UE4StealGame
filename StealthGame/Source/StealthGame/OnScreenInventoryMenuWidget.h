// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIInterface.h"
#include "OnScreenInventoryMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UOnScreenInventoryMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupMenuInterface(IUIInterface* UIInterface);
	void Setup();
	UFUNCTION()
		void ChangePillText(FString InText);
protected:
	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
private:
	IUIInterface * UIInterface;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PillText;


	
	
};
