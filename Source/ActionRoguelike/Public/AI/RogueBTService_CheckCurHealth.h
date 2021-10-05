// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RogueBTService_CheckCurHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URogueBTService_CheckCurHealth : public UBTService
{
	GENERATED_BODY()
	
	protected:
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector CurrentHealthKey;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
