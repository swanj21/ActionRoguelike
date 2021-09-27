// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueProjectileTeleport.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARogueProjectileTeleport : public ARogueProjectile
{
	GENERATED_BODY()

	public:
	ARogueProjectileTeleport();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	class UParticleSystemComponent* DestructionEffectComponent;

	protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnComponentHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	void Explode_TimeElapsed();
	void DoProjectileExplosion();
	void Teleport_TimeElapsed();
	void DoTeleportPlayer(FVector Location);

	FTimerHandle TimerHandle_Explode;
	FTimerHandle TimerHandle_Teleport;
	
};
