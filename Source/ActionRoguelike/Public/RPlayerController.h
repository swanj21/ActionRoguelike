// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARPlayerController : public APlayerController {
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	/**
	 * Listen for the incoming player state(for clients this may be nullptr when initially joining a game,
	 * afterwards the player state will not change again as PlayerControllers maintain the same player state throughout
	 * the level.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateChanged;

	virtual void SetPawn(APawn* InPawn) override;

	/**
	 * Called when a player controller is ready to begin playing, good moment to initialize things like UI which might be
	 * too early in BeginPlay(i.e. Multiplayer clients where not all data could have been received yet)
	 */
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	void OnRep_PlayerState() override;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	void SetupInputComponent() override;
};
