// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor,
                                              class URogueAttributeComponent*, OwningComponent, float, NewHealth, float,
                                              Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, URogueAttributeComponent*,
                                              OwningComponent, float, NewRage, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueAttributeComponent : public UActorComponent {
	GENERATED_BODY()

public:
	URogueAttributeComponent();

	// -------------------- //
	// ------ HEALTH ------ //
	// -------------------- //
protected:
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category="Health")
	float Health;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category="Health")
	float MaxHealth;

	/**
	* Value for an actor to be considered 'low health'
	*/
	UPROPERTY(EditAnywhere, Category="Health")
	float LowHealthThreshold;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

public:
	UFUNCTION(BlueprintCallable, Category="Health")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsLowHealth() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealth() {
		return Health;
	}

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() {
		return MaxHealth;
	}

	UFUNCTION(BlueprintCallable, Category="Health")
	bool Kill(AActor* InstigatorActor);

	// ------------------ //
	// ------ RAGE ------ //
	// ------------------ //
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category="Rage")
	int32 Rage;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category="Rage")
	int32 MaxRage;

	UFUNCTION(BlueprintCallable, Category="Rage")
	int32 GetRage() {
		return Rage;
	}

	UFUNCTION(BlueprintCallable, Category="Rage")
	int32 GetMaxRage() {
		return MaxRage;
	}

	UFUNCTION(BlueprintCallable, Category="Rage")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UPROPERTY(BlueprintAssignable, Category="Rage")
	FOnRageChanged OnRageChanged;

	UFUNCTION(NetMulticast, Unreliable) // Unreliable because the changes are cosmetic only
	void MulticastRageChanged(AActor* InstigatorActor, float Delta);

	// -------------------- //
	// ------ STATIC ------ //
	// -------------------- //
public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	static URogueAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Health", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
};