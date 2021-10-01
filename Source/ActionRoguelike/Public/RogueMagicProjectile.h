// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARogueMagicProjectile : public ARogueProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARogueMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float Damage = 20.f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
