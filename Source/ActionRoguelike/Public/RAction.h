// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URAction : public UObject
{
	GENERATED_BODY()

public:
	URAction();

	// Action nickname to start/stop without a reference to the object
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool IsRunning() const;

	UWorld* GetWorld() const override;

	protected:
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category="Action")
	class URActionComponent* GetOwningComponent() const;

	bool bIsRunning;
};
