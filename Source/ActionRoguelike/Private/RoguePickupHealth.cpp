// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickupHealth.h"

#include "RGameplayFunctionLibrary.h"
#include "RogueAttributeComponent.h"
#include "RPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ARoguePickupHealth::ARoguePickupHealth() {
	CreditCost = 50.f;
	HealValue = 50.f;
}

void ARoguePickupHealth::Interact_Implementation(APawn* InstigatorPawn) {
	if (!InstigatorPawn || !bIsActive) {
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

FText ARoguePickupHealth::GetInteractText_Implementation(APawn* InstigatorPawn) {
	URogueAttributeComponent* AttributeComp = URogueAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->GetHealth() == AttributeComp->GetMaxHealth()) {
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health");
	}

	// Without #define LOCTEXT_NAMESPACE -
	// return FText::Format(NSLOCTEXT("InteractableActors", "HealthPotion_InteractMessage", "Cost {0} Credits.  Restore health to maximum"), CreditCost);
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits.  Restore health to maximum"), CreditCost);
}

bool ARoguePickupHealth::HasEnoughCredits(AController* Controller) {
	ARPlayerState* PlayerState = Controller->GetPlayerState<ARPlayerState>();
	if (ensure(PlayerState)) {
		return PlayerState->GetCurrentCredits() >= CreditCost;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE