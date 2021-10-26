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

void ARPlayerState::OnRep_Credits(float OldCredits) {
	OnCreditsChanged.Broadcast(this, CurrentCredits, CurrentCredits - OldCredits);
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

bool ARPlayerState::UpdateCredits(float Amount) {
	if (Amount == 0) {
		return false;
	}
	CurrentCredits += Amount;
	CurrentCredits = FMath::Clamp(CurrentCredits, 0.f, MaxCredits);
	
	return true;
}

void ARPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARPlayerState, CurrentCredits);
}