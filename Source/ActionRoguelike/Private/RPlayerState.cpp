// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerState.h"

ARPlayerState::ARPlayerState() {
	MaxCredits = 1000.f;
	CurrentCredits = 0.f;
}

float ARPlayerState::GetCurrentCredits() {
	return CurrentCredits;
}

float ARPlayerState::GetMaxCredits() {
	return MaxCredits;
}

bool ARPlayerState::UpdateCredits(float Amount) {
	UE_LOG(LogTemp, Warning, TEXT("Updating credits in player state"))
	if (Amount == 0) {
		return false;
	}
	float OldAmount = CurrentCredits;
	CurrentCredits += Amount;
	CurrentCredits = FMath::Clamp(CurrentCredits, 0.f, MaxCredits);

	float ActualDelta = CurrentCredits - OldAmount;

	OnCreditsChanged.Broadcast(this, CurrentCredits, ActualDelta);
	
	return true;
}
