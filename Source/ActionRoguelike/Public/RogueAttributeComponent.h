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
	URogueAttributeComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	float Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth = 100;

	/**
	* Value for an actor to be considered 'low health'
	*/
	UPROPERTY(EditAnywhere, Category="Health")
	float LowHealthThreshold;
	
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
	float GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category="Health")
	bool Kill(AActor* InstigatorActor);

	// ------
	// STATIC
	// ------
	public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	static URogueAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Health", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
};
