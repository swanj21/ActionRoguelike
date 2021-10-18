// Fill out your copyright notice in the Description page of Project Settings.


#include "RActionEffect_Thorns.h"

#include "RGameplayFunctionLibrary.h"
#include "RogueAttributeComponent.h"

URActionEffect_Thorns::URActionEffect_Thorns() {
	// Since this acts as a permanent buff for the player, it has no timed duration or period
	Duration = 0;
	Period = 0;
	ActionName = "Thorns";

	PercentDamageToReflect = 20.f;
}

void URActionEffect_Thorns::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);

	URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(Instigator);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &URActionEffect_Thorns::DamageInstigator);
}

void URActionEffect_Thorns::StopAction_Implementation(AActor* Instigator) {
	Super::StopAction_Implementation(Instigator);

	URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(Instigator);
	AttributeComponent->OnHealthChanged.RemoveDynamic(this, &URActionEffect_Thorns::DamageInstigator);
}

void URActionEffect_Thorns::DamageInstigator(AActor* InstigatorActor, URogueAttributeComponent* OwningComponent, float NewHealth, float Delta) {
	if (bIsActivating) {
		return;
	}
	if (InstigatorActor != OwningComponent->GetOwner()) {
		int32 DamageToReflect = FMath::Abs(Delta) * (PercentDamageToReflect / 100.f);
		bIsActivating = true;
		URGameplayFunctionLibrary::ApplyDamage(OwningComponent->GetOwner(), InstigatorActor, DamageToReflect);
		bIsActivating = false;
	}
}