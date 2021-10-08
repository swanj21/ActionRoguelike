// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "RogueGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARogueGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	public:
	ARogueGameModeBase();
	virtual void StartPlay() override;

	protected:
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	TSubclassOf<AActor> CoinClass;

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	TSubclassOf<AActor> HealthPotionClass;
	
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float MinionSpawnTimeInterval;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float MaxBotsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	float MaxCoinsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	float MaxHealthToSpawn;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	bool bShouldSpawnBotsFromCurve;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UEnvQuery* SpawnCoinQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UEnvQuery* SpawnHealthQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UCurveFloat* DifficultyCurve;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnMinionSpawnQueryComplete(class UEnvQueryInstanceBlueprintWrapper* QueryInstance,
		EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(BlueprintCallable, Category="Pickup")
	void SpawnCoinsAtStart();

	UFUNCTION()
	void OnCoinSpawnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
		EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(BlueprintCallable, Category="Pickup")
	void SpawnHealthAtStart();

	UFUNCTION()
	void OnHealthSpawnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
		EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerTimeElapsed(AController* Controller);

	public:
	UFUNCTION(Exec)
	void KillAllBots();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
};
