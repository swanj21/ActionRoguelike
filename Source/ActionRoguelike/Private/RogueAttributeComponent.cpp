// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeComponent.h"

#include "RogueGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageModifier(
	TEXT("rogue.DamageMultiplier"), 1.f, TEXT("Global damage modifier for attribute component"), ECVF_Cheat);

// Sets default values for this component's properties
URogueAttributeComponent::URogueAttributeComponent() {
	Health = 100.f;
	MaxHealth = 100.f;

	Rage = 0;
	MaxRage = 100;
}

bool URogueAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta) {
	if (!GetOwner()->CanBeDamaged() && Delta < 0.f) {
		return false;
	}

	if (Health == MaxHealth && Delta > 0.f) {
		return false;
	}

	if (Delta < 0.f) {
		float DamageMultiplier = CVarDamageModifier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	float OldHealth = Health;

	Health += Delta;

	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Died
	if (ActualDelta < 0.f && Health <= 0.f) {
		ARogueGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARogueGameModeBase>();
		if (GameMode) {
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
			return true;
		}
	}

	// Rage calculation (Rage given = 1/5th of health lost)
	const int32 RageToAdd = FMath::Abs(ActualDelta) / 5;
	int32 OldRage = Rage;
	Rage += RageToAdd;

	OnRageChanged.Broadcast(InstigatorActor, this, Rage, Rage - OldRage);

	return true;
}

bool URogueAttributeComponent::IsAlive() const {
	return Health > 0.f;
}

bool URogueAttributeComponent::IsLowHealth() const {
	return Health <= LowHealthThreshold;
}

bool URogueAttributeComponent::Kill(AActor* InstigatorActor) {
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

bool URogueAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta) {
	if (Rage == MaxRage && Delta > 0.f) {
		return false;
	}

	int32 OldRage = Rage;

	Rage += Delta;
	Rage = FMath::Clamp(Rage, 0, MaxRage);

	int32 ActualDelta = Rage - OldRage;

	OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);

	return true;
}

// ----------------
// Static functions
// ----------------
URogueAttributeComponent* URogueAttributeComponent::GetAttributes(AActor* FromActor) {
	if (FromActor) {
		return Cast<URogueAttributeComponent>(FromActor->GetComponentByClass(URogueAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool URogueAttributeComponent::IsActorAlive(AActor* Actor) {
	if (Actor) {
		URogueAttributeComponent* AttributeComponent = GetAttributes(Actor);
		if (AttributeComponent) {
			return AttributeComponent->IsAlive();
		}
	}
	return false;
}
