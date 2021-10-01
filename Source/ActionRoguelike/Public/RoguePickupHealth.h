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

	void Interact_Implementation(APawn* InstigatorPawn) override;

	protected:
	void DisableItem() override;
	void EnableItem() override;
};
