// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguePickup.h"
#include "RPickupAbility.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARPickupAbility : public ARoguePickup
{
	GENERATED_BODY()

	protected:
	ARPickupAbility();

	virtual void DisableItem() override;

	void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Action")
	TSubclassOf<class URAction> ActionToGive;
};
