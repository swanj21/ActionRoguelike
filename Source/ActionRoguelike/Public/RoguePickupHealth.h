// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguePickup.h"
#include "RoguePickupHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARoguePickupHealth : public ARoguePickup
{
	GENERATED_BODY()

	ARoguePickupHealth();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	protected:
	UPROPERTY(EditAnywhere, Category="Credits")
	float CreditCost;
	
	UPROPERTY(EditAnywhere, Category="Healing")
	float HealValue;

	UFUNCTION(BlueprintCallable, Category="Credits")
	bool HasEnoughCredits(class AController* Controller);
};
