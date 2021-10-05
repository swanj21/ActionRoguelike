// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "RogueQueryContext_TargetActor.generated.h"

/**
 * UNUSED
 * 
 * This is implemented in blueprints and I could not figure out how to translate the blueprint ProvideSingleActor()
 * to this class' ProvideContext() method.
 */
UCLASS()
class ACTIONROGUELIKE_API URogueQueryContext_TargetActor : public UEnvQueryContext
{
	GENERATED_BODY()

	protected:
	URogueQueryContext_TargetActor();

	/**
	 * Get the Target Actor value from the current AI character's blackboard
	 */
	UFUNCTION(BlueprintCallable, Category="Query")
	AActor* ProvideSingleActor(UObject* QuerierObject, AActor* QuerierActor, AActor*& ResultingActor) const;

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

	FName TargetActorKey;
};
