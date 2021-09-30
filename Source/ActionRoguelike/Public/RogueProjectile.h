// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARogueProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARogueProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	class UParticleSystemComponent* EffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Audio")
	class UAudioComponent* FlightAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	class USoundCue* ImpactSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
