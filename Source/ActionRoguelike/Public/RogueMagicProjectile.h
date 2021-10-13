// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag ParryTag;
	
	UFUNCTION(BlueprintCallable)
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float Damage;

	UPROPERTY(BlueprintReadOnly, Category="Damage")
	AActor* HitActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<class URActionEffect> BurningActionClass;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
