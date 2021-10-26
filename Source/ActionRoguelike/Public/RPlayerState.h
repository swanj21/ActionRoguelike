// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, class ARPlayerState*, AffectedPlayerState, float,
                                               NewCredits, float, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARPlayerState : public APlayerState {
	GENERATED_BODY()

public:
	ARPlayerState();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category="Credits")
	bool UpdateCredits(float Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Credits")
	float GetCurrentCredits();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Credits")
	float GetMaxCredits();

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(class URSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(URSaveGame* SaveObject);

protected:
	UPROPERTY(BlueprintAssignable, Category="Credits")
	FOnCreditsChanged OnCreditsChanged;

	UPROPERTY(ReplicatedUsing="OnRep_Credits", EditAnywhere, Category="Credits")
	float CurrentCredits;

	UPROPERTY(EditDefaultsOnly, Category="Credits")
	float MaxCredits;

	UFUNCTION()
	void OnRep_Credits(float OldCredits);

	// UFUNCTION(NetMulticast, Unreliable)
	// void MulticastCreditsChanged(float Delta);
};
