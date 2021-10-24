// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, class ARPlayerState*, AffectedPlayerState, float, NewCredits, float, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARPlayerState : public APlayerState
{
	GENERATED_BODY()

	public:
	ARPlayerState();
	
	virtual void Tick(float DeltaSeconds) override;

	// ------- //
	// CREDITS //
	// ------- //
	public:
	
	UFUNCTION(BlueprintCallable, Category="Credits")
	bool UpdateCredits(float Amount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Credits")
	float GetCurrentCredits();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Credits")
	float GetMaxCredits();
	
	protected:
	
	UPROPERTY(BlueprintAssignable, Category="Credits")
	FOnCreditsChanged OnCreditsChanged;
	
	UPROPERTY(Replicated, EditAnywhere, Category="Credits")
	float CurrentCredits;

	UPROPERTY(EditDefaultsOnly, Category="Credits")
	float MaxCredits;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCreditsChanged(float Delta);
};
