// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.h"
#include "InGameMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FailMenuWidget.h"
#include "SuccessMenuWidget.h"
#include "LastLevelMenuWidget.h"
#include "HintMenuWidget.h"
#include "OnScreenInventoryMenuWidget.h"
#include "Engine.h"
#include "MySaveGame.h"
#include "StealthGameCharacter.h"
#include "LoadingScreenWidget.h"


UStealthGameInstance::UStealthGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	//ConstructorHelpers::FClassFinder<USoundBase> SoundClass(TEXT("SoundWave'/Game/Audio/ButtonSound.ButtonSound'"));

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
	InGameMenuClass = InGameMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> FailMenuBPClass(TEXT("/Game/MenuSystem/WBP_FailMenu"));
	if (!ensure(FailMenuBPClass.Class != nullptr)) return;
	FailMenuClass = FailMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> SuccessMenuBPClass(TEXT("/Game/MenuSystem/WBP_SuccessMenu"));
	if (!ensure(SuccessMenuBPClass.Class != nullptr)) return;
	SuccessMenuClass = SuccessMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> LastLevelMenuBPClass(TEXT("/Game/MenuSystem/WBP_LastLevelMenu"));
	if (!ensure(LastLevelMenuBPClass.Class != nullptr)) return;
	LastLevelMenuClass = LastLevelMenuBPClass.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> PopupHintMenuBPClass(TEXT("/Game/MenuSystem/WBP_PopupHint"));
	if (!ensure(PopupHintMenuBPClass.Class != nullptr)) return;
	PopupHintMenuClass = PopupHintMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> OnScreenMenuBPClass(TEXT("/Game/MenuSystem/WBP_OnScreenInventory"));
	if (!ensure(OnScreenMenuBPClass.Class != nullptr)) return;
	OnScreenMenuClass = OnScreenMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenMenuBPClass(TEXT("/Game/MenuSystem/WBP_LoadindScreen"));
	if (!ensure(LoadingScreenMenuBPClass.Class != nullptr)) return;
	LoadingScreenMenuClass = LoadingScreenMenuBPClass.Class;

	//Find reference and assign for ButtonClickSound
	ConstructorHelpers::FObjectFinder<USoundBase> ButtonClickSoundClass(TEXT("/Game/Audio/ButtonSound"));
	if (ButtonClickSoundClass.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Button Click sound object"));
	}
	ButtonClickSound = ButtonClickSoundClass.Object;
	//

	//Find reference and assign for WinSound
	ConstructorHelpers::FObjectFinder<USoundBase> WinSoundClass(TEXT("/Game/Audio/WinSound"));
	if (WinSoundClass.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Win sound object"));
	}
	WinSound = WinSoundClass.Object;
	//

		//Find reference and assign for LoseSound
	ConstructorHelpers::FObjectFinder<USoundBase> LoseSoundClass(TEXT("/Game/Audio/LoseSound"));
	if (LoseSoundClass.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Lose sound object"));
	}
	LoseSound = LoseSoundClass.Object;
	//

	Difficulty = "Standard";

}

void UStealthGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Game instance Init"));
	UE_LOG(LogTemp, Warning, TEXT("found class %s"), *MenuClass->GetName());


}

void UStealthGameInstance::StartGame(FString LevelName)
{

	PlayButtonClickSound();
	LoadLoadingScreen();
	if (Difficulty == "Standard")
	{
		FString TheString = "Standard_" + LevelName;
		LevelToOpen = FName(*TheString);
	}
	else if (Difficulty == "Expert")
	{
		FString TheString = "Expert_" + LevelName;
		LevelToOpen = FName(*TheString);
	}
	GetWorld()->GetTimerManager().SetTimer(DelayStartGameHandle, this, &UStealthGameInstance::OpenTheLevel, 3.0f, false, ButtonClickSound->Duration/2);
	
}

void UStealthGameInstance::OpenTheLevel()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayStartGameHandle);
	UGameplayStatics::OpenLevel(GetWorld(), LevelToOpen);
}

void UStealthGameInstance::LoadTheGame()
{

	UGameplayStatics::OpenLevel(GetWorld(), "Level1");

}

void UStealthGameInstance::Level1Completed()
{
	UMySaveGame* MySaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if (MySaveGameInstance->LevelCompleted < 1)
	{
		MySaveGameInstance->LevelCompleted = 1;
		UGameplayStatics::SaveGameToSlot(MySaveGameInstance, TEXT("MySlot"),0);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Level 1 Completed, Saved to the progress file"));
	}
}

void UStealthGameInstance::Level2Completed()
{
	UMySaveGame* MySaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if (MySaveGameInstance->LevelCompleted < 2)
	{
		MySaveGameInstance->LevelCompleted = 2;
		UGameplayStatics::SaveGameToSlot(MySaveGameInstance, TEXT("MySlot"), 0);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Level 2 Completed, Saved to the progress file"));
	}	
}



void UStealthGameInstance::QuitGame()
{
	GetWorld()->GetTimerManager().SetTimer(DelayQuitGameHandle, this, &UStealthGameInstance::QuitTheGame, 3.0f, false, ButtonClickSound->Duration / 2);
}

void UStealthGameInstance::QuitTheGame()
{
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;
	GetWorld()->GetTimerManager().ClearTimer(DelayQuitGameHandle);
	playerController->ConsoleCommand("quit");
}

void UStealthGameInstance::LoadMainMenu()
{
	auto MainMenuInstance = CreateWidget<UMainMenuWidget>(this, MenuClass);
	MainMenuInstance->SetupMenuInterface(this);
}

void UStealthGameInstance::LoadInGameMenu()
{
	auto InGameMenuInstance = CreateWidget<UInGameMenuWidget>(this, InGameMenuClass);
	InGameMenuInstance->SetupMenuInterface(this);
}

void UStealthGameInstance::CancelMenu()
{
	//Being done inside In Game Menu widget;
	PlayButtonClickSound();
}

void UStealthGameInstance::BackToMainMenu()
{
	PlayButtonClickSound();
	LoadLoadingScreen();
	GetWorld()->GetTimerManager().SetTimer(DelayBackToMainMenuHandle, this, &UStealthGameInstance::BackToTheMainMenu, 3.0f, false, ButtonClickSound->Duration / 2);

}

void UStealthGameInstance::BackToTheMainMenu()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayBackToMainMenuHandle);
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void UStealthGameInstance::RestartLevel()
{
	PlayButtonClickSound();
	LoadLoadingScreen();
	GetWorld()->GetTimerManager().SetTimer(DelayRestartLevelMenuHandle, this, &UStealthGameInstance::RestartTheLevel, 3.0f, false, ButtonClickSound->Duration / 2);
}

void UStealthGameInstance::RestartTheLevel()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayRestartLevelMenuHandle);
	FString CurrentLevelName = GetWorld()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("current level name is %s"), *CurrentLevelName);
	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
}

void UStealthGameInstance::LoadNextLevel()
{
	PlayButtonClickSound();
	LoadLoadingScreen();
	GetWorld()->GetTimerManager().SetTimer(DelayLoadNextLevelMenuHandle, this, &UStealthGameInstance::LoadTheNextLevel, 3.0f, false, ButtonClickSound->Duration / 2);
}

void UStealthGameInstance::LoadTheNextLevel()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayLoadNextLevelMenuHandle);
	FString CurrentLevelName = GetWorld()->GetName();

	if (CurrentLevelName == "Standard_Level1")
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Standard_Level2");
	}
	else if (CurrentLevelName == "Standard_Level2")
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Standard_Level3");
	}
	else if (CurrentLevelName == "Standard_Level3")
	{
		//Last Level!!

	}
	else if (CurrentLevelName == "Expert_Level1")
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Expert_Level2");
	}
	else if (CurrentLevelName == "Expert_Level2")
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Expert_Level3");
	}
	else if (CurrentLevelName == "Expert_Level3")
	{
		//Last Level!!
	}
}

void UStealthGameInstance::LoadFailMenu()
{
	UGameplayStatics::PlaySound2D(GetWorld(), LoseSound);
	auto FailMenuClassInstance = CreateWidget<UFailMenuWidget>(this, FailMenuClass);
	FailMenuClassInstance->SetupMenuInterface(this);
}

void UStealthGameInstance::LoadSuccessMenu()
{
	UGameplayStatics::PlaySound2D(GetWorld(), WinSound);
	FString CurrentLevelName = GetWorld()->GetName();
	if (CurrentLevelName == "Expert_Level3" || CurrentLevelName == "Standard_Level3")
	{
		//Last Level
		//Create and Load Last Level UI up 
		auto LastLevelMenuInstance = CreateWidget<ULastLevelMenuWidget>(this, LastLevelMenuClass);
		LastLevelMenuInstance->SetupMenuInterface(this);
		return;

	}
	auto SuccessMenuClassInstance = CreateWidget<USuccessMenuWidget>(this, SuccessMenuClass);
	SuccessMenuClassInstance->SetupMenuInterface(this);
}
void UStealthGameInstance::LoadOnScreenMenu()
{
	PopupHintMenuInstance = CreateWidget<UOnScreenInventoryMenuWidget>(this, OnScreenMenuClass);
}

void UStealthGameInstance::OnStandardDifficultyButtonClicked(FString NextLevelName)
{
	FName ConvertedFString = FName(*NextLevelName);
	UGameplayStatics::OpenLevel(GetWorld(), ConvertedFString);
}

void UStealthGameInstance::OnExpertDifficultyButtonClicked(FString NextLevelName)
{
	FName ConvertedFString = FName(*NextLevelName);
	UGameplayStatics::OpenLevel(GetWorld(), ConvertedFString);
}

void UStealthGameInstance::LoadHintMenu()
{
	auto PopupHintMenuInstance = CreateWidget<UHintMenuWidget>(this, PopupHintMenuClass);
	//PopupHintMenuInstance->SetupMenuInterface(this);
}


void UStealthGameInstance::ChangeOnScreenPillText(FString InText)
{
	PopupHintMenuInstance->ChangePillText(InText);
}

void UStealthGameInstance::ShowCompletedLevelMenu()
{
	UMySaveGame* MySaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	MySaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
	if (MySaveGameInstance != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Loading Level Completed progress from file"));
		int32 CurrentCompletedLevelProgress = MySaveGameInstance->LevelCompleted;
		//Disable buttons as needed! based on current complted level progress
	}

}

void UStealthGameInstance::SetDifficulty(FString Difficulty)
{
	this->Difficulty = Difficulty;
}

void UStealthGameInstance::CheatCode(int32 InCode)
{
	if (InCode == 456753)
	{
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		if (!ensure(playerController != nullptr)) return;
		AStealthGameCharacter* MyCharacter = Cast<AStealthGameCharacter>(playerController->GetCharacter());
		if (MyCharacter != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cheat code applied, Change player invisible time to 300"));
			MyCharacter->InvisibleTime = 300;
		}
	}
}

void UStealthGameInstance::LoadLoadingScreen()
{
	auto LoadingScreenMenuInstance = CreateWidget<ULoadingScreenWidget>(this, LoadingScreenMenuClass);
	LoadingScreenMenuInstance->SetupMenuInterface(this);
}

void UStealthGameInstance::PlayButtonClickSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound, 0.2f);
}


