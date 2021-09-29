// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, class URogueAttributeComponent*, OwningComponent, float, NewHealth, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueAttributeComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URogueAttributeComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health = 100;

	// HealthMax, Stamina, Strength, CriticalHitChance
public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(float Delta);

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsAlive() const;
};
