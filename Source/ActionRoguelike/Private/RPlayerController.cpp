// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ARPlayerController::SetPawn(APawn* InPawn) {
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ARPlayerController::BeginPlayingState() {
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
}

void ARPlayerController::OnRep_PlayerState() {
	Super::OnRep_PlayerState();

	OnPlayerStateChanged.Broadcast(PlayerState);
}

void ARPlayerController::TogglePauseMenu() {
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport()) {
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		
		UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance) {
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		
		UGameplayStatics::SetGlobalTimeDilation(this, .01f);
	}
}

void ARPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this, &ARPlayerController::TogglePauseMenu);
}
