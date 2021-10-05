// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RogueBTTask_DoHeal.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URogueBTTask_DoHeal : public UBTTaskNode
{
	GENERATED_BODY()
	public:
	URogueBTTask_DoHeal();
	
	protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
