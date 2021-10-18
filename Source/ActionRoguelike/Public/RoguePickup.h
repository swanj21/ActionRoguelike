// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RoguePickup.generated.h"

/**
 * Base class for anything that can be picked up by the player 
 */
UCLASS()
class ACTIONROGUELIKE_API ARoguePickup : public AActor, public IRogueGameplayInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
public:	
	// Sets default values for this actor's properties
	ARoguePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	FTimerHandle EnableItemTimerHandle;

	UPROPERTY(EditAnywhere, Category="Timer")
	float RespawnTimer;

	UFUNCTION(BlueprintCallable, Category="Activation")
	virtual void DisableItem();

	UFUNCTION(BlueprintCallable, Category="Activation")
	virtual void EnableItem();

	bool IsActive = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
