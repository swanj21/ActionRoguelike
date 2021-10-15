// Fill out your copyright notice in the Description page of Project Settings.


#include "RPickupAbility.h"

#include "RAction.h"
#include "RActionComponent.h"
#include "RogueAttributeComponent.h"

ARPickupAbility::ARPickupAbility() {
	
}

void ARPickupAbility::DisableItem() {
	Super::DisableItem();
	
	Destroy();
}

void ARPickupAbility::Interact_Implementation(APawn* InstigatorPawn) {
	 if (!ActionToGive) {
		UE_LOG(LogTemp, Error, TEXT("There is no action to give!"))
	 	return;
	 }
	URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(InstigatorPawn);

	if (AttributeComponent) {
		AActor* ComponentOwner = AttributeComponent->GetOwner();
		URActionComponent* ActionComponent = Cast<URActionComponent>(ComponentOwner->GetComponentByClass(URActionComponent::StaticClass()));
		if (ActionComponent && ActionComponent->AddAction(InstigatorPawn, ActionToGive)) {
			DisableItem();
		}
	}
}
