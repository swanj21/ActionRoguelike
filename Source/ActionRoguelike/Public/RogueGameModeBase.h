// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "RogueGameModeBase.generated.h"

USTRUCT()
struct FMonsterInfoRow : public FTableRowBase {
	GENERATED_BODY()

public:
	FMonsterInfoRow() {
		Weight = 1.f;
		SpawnCost = 5.f;
		KillReward = 5.f;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	/* Relative chance to pick this monster */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	/* Points required by gamemode to spawn this unit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	/* Amount of credits awarded to killer of this unit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;	
};
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARogueGameModeBase : public AGameModeBase {
	GENERATED_BODY()

public:
	ARogueGameModeBase();
	
	virtual void StartPlay() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UDataTable* MonsterTable;
	
	FString SlotName;
	
	UPROPERTY()
	class URSaveGame* CurrentSaveGame;
	
	// UPROPERTY(EditDefaultsOnly, Category="AI")
	// TSubclassOf<AActor> MinionClass;

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

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float PlayerRespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UEnvQuery* SpawnCoinQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UEnvQuery* SpawnHealthQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	float RequiredObjectDistance;

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

	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

public:
	UFUNCTION(Exec)
	void KillAllBots();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
};
