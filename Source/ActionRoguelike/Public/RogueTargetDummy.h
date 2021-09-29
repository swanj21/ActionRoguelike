// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueTargetDummy.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARogueTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARogueTargetDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class URogueAttributeComponent* AttributeComponent;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void OnHealthChanged(AActor* InstigatorActor, URogueAttributeComponent* OwningComponent, float NewHealth, float Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
