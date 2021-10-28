// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RAction.generated.h"

USTRUCT()
struct FActionRepData {
	GENERATED_BODY()

public:
	// Struct variables are replicated by default, need to annotate with NotReplicated if you do not want it to replicate.
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};
/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URAction : public UObject
{
	GENERATED_BODY()

public:
	URAction();

	void Initialize(class URActionComponent* NewActionComponent);

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

	/* Should start immediately when added to an action component */
	UPROPERTY(EditDefaultsOnly, Category="Action")
	bool bAutoStart;

	UWorld* GetWorld() const override;

	/* Need to set this to true so this UObject can be replicated over the network */
	bool IsSupportedForNetworking() const override { return true; }
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
	URActionComponent* ActionComp;
	
	UFUNCTION(BlueprintCallable, Category="Action")
	URActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UPROPERTY(Replicated)
	float TimeStarted;

	UFUNCTION()
	void OnRep_RepData();
};
