// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguePickup.h"
#include "RPickupCoin.generated.h"

/**
 * A coin pickup to give the player credits when overlapped
 */
UCLASS()
class ACTIONROGUELIKE_API ARPickupCoin : public ARoguePickup
{
	GENERATED_BODY()

	protected:
	ARPickupCoin();

	UPROPERTY(EditAnywhere, Category="Credits")
	float CreditsToGive;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
