// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RogueAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARogueAIController : public AAIController
{
	GENERATED_BODY()

	protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UBehaviorTree* BehaviorTree;
};
