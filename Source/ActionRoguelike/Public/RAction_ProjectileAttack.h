// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RAction.h"
#include "RAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URAction_ProjectileAttack : public URAction
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	class UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, Category="Effects")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float AttackAnimDelay;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
	
	public:
	URAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
};
