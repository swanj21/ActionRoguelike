// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URActionComponent();

	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(TSubclassOf<class URAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<class URAction*> Actions;

	/* Abilities given at game start */
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<URAction>> DefaultActions;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
