// Fill out your copyright notice in the Description page of Project Settings.


#include "RPickupCoin.h"

#include "RGameplayFunctionLibrary.h"

ARPickupCoin::ARPickupCoin() {
	CreditsToGive = 5.f;
}

void ARPickupCoin::Interact_Implementation(APawn* InstigatorPawn) {
	if (!bIsActive || !InstigatorPawn) { return; }
	
	if (URGameplayFunctionLibrary::GiveCredits(InstigatorPawn->GetController(), CreditsToGive)) {
		DisableItem();
		GetWorldTimerManager().SetTimer(EnableItemTimerHandle, this, &ARPickupCoin::EnableItem, RespawnTimer);
	}
}
