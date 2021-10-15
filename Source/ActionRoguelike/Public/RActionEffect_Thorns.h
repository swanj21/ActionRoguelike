// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RActionEffect.h"
#include "RActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URActionEffect_Thorns : public URActionEffect
{
	GENERATED_BODY()

	public:
	URActionEffect_Thorns();

	protected:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float PercentDamageToReflect;

	private:
	UFUNCTION(BlueprintCallable, Category="Damage")
	void DamageInstigator(AActor* InstigatorActor, class URogueAttributeComponent* OwningComponent, float NewHealth, float Delta);	
};
