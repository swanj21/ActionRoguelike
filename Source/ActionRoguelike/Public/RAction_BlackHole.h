// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RAction_ProjectileAttack.h"
#include "RAction_BlackHole.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URAction_BlackHole : public URAction_ProjectileAttack
{
	GENERATED_BODY()

	protected:
	URAction_BlackHole();
	
	bool CanStart_Implementation(AActor* Instigator) override;

	void StartAction_Implementation(AActor* Instigator) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cost")
	int32 RageCost;
};
