// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickupHealth.h"

#include "RGameplayFunctionLibrary.h"
#include "RogueAttributeComponent.h"
#include "RPlayerState.h"

ARoguePickupHealth::ARoguePickupHealth() {
	CreditCost = 50.f;
	HealValue = 50.f;
}

void ARoguePickupHealth::Interact_Implementation(APawn* InstigatorPawn) {
	if (!InstigatorPawn || !IsActive) {
		return;
	}

	AController* Controller = InstigatorPawn->GetInstigatorController();
	if (!Controller) {
		return;
	}

	if(!HasEnoughCredits(Controller)) {
		UE_LOG(LogTemp, Error, TEXT("Player %s does not have enough credits!"), *GetNameSafe(Controller))
		return;
	}
	URogueAttributeComponent* AttributeComponent = Cast<URogueAttributeComponent>(InstigatorPawn->GetComponentByClass(URogueAttributeComponent::StaticClass()));
	
	if (AttributeComponent && AttributeComponent->ApplyHealthChange(this, HealValue)) {
		URGameplayFunctionLibrary::TakeCredits(Controller, CreditCost);
		DisableItem();
		GetWorldTimerManager().SetTimer(EnableItemTimerHandle, this, &ARoguePickupHealth::EnableItem, RespawnTimer);
	}	
}

bool ARoguePickupHealth::HasEnoughCredits(AController* Controller) {
	ARPlayerState* PlayerState = Controller->GetPlayerState<ARPlayerState>();
	if (ensure(PlayerState)) {
		return PlayerState->GetCurrentCredits() >= CreditCost;
	}
	return false;
}
