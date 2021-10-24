// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerState.h"

#include "RSaveGame.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

ARPlayerState::ARPlayerState() {
	PrimaryActorTick.bCanEverTick = true;
	
	MaxCredits = 1000.f;
	CurrentCredits = 0.f;
}

void ARPlayerState::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	//LogOnScreen(this, FString::Printf(TEXT("Credits: %f"), CurrentCredits), FColor::Green, 0.f);
}

float ARPlayerState::GetCurrentCredits() {
	return CurrentCredits;
}

float ARPlayerState::GetMaxCredits() {
	return MaxCredits;
}

void ARPlayerState::SavePlayerState_Implementation(URSaveGame* SaveObject) {
	if (SaveObject) {
		SaveObject->Credits = CurrentCredits;
	}
}

void ARPlayerState::LoadPlayerState_Implementation(URSaveGame* SaveObject) {
	if (SaveObject) {
		CurrentCredits = SaveObject->Credits;
	}
}

void ARPlayerState::MulticastCreditsChanged_Implementation(float Delta) {
	OnCreditsChanged.Broadcast(this, CurrentCredits, Delta);
}

bool ARPlayerState::UpdateCredits(float Amount) {
	if (Amount == 0) {
		return false;
	}
	float OldAmount = CurrentCredits;
	CurrentCredits += Amount;
	CurrentCredits = FMath::Clamp(CurrentCredits, 0.f, MaxCredits);

	float ActualDelta = CurrentCredits - OldAmount;

	if (ActualDelta != 0.f) {
		MulticastCreditsChanged(ActualDelta);
	}
	
	return true;
}

void ARPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARPlayerState, CurrentCredits);
}