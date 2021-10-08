// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "RGameplayFunctionLibrary.h"
#include "RogueAttributeComponent.h"
#include "RogueCharacter.h"
#include "AI/RogueAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rogue.SpawnBots"), true, TEXT("Enable spawning of bots via a timer"), ECVF_Cheat);

ARogueGameModeBase::ARogueGameModeBase() {
	MinionSpawnTimeInterval = 2.f;
	MaxBotsToSpawn = 10.f;
	MaxCoinsToSpawn = 10.f;
	MaxHealthToSpawn = 3.f;
	PlayerRespawnDelay = 2.f;
}

void ARogueGameModeBase::StartPlay() {
	Super::StartPlay();

	UE_LOG(LogTemp, Warning, TEXT("Spawning items"))
	SpawnHealthAtStart();
	SpawnCoinsAtStart();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ARogueGameModeBase::SpawnBotTimerElapsed, MinionSpawnTimeInterval, true);
}

void ARogueGameModeBase::SpawnBotTimerElapsed() {
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		GetWorld(),
		SpawnBotQuery,
		this, 
		EEnvQueryRunMode::RandomBest5Pct,
		nullptr);

	if (!CVarSpawnBots.GetValueOnGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."))
		return;
	}

	int32 CurAliveBots = 0;
	for (TActorIterator<ARogueAICharacter> It(GetWorld()); It; ++It) {
		ARogueAICharacter* Bot = *It;

		URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive()) {
			CurAliveBots++;
		}
	}

	if (DifficultyCurve && bShouldSpawnBotsFromCurve) {
		MaxBotsToSpawn = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if (CurAliveBots >= MaxBotsToSpawn) {
		return;
	}
	
	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARogueGameModeBase::OnMinionSpawnQueryComplete);
	}
}

void ARogueGameModeBase::OnMinionSpawnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Error, TEXT("Spawn bot EQS query failed"))
		return;
	}
	
	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);

	if (Locations.IsValidIndex(0)) {
		UE_LOG(LogTemp, Log, TEXT("Spawning new bot"))
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 30.f, 8, FColor::Blue, true, 5.f,0, 3.f);
	}
}

void ARogueGameModeBase::SpawnCoinsAtStart() {
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
	GetWorld(),
	SpawnCoinQuery,
	this, 
	EEnvQueryRunMode::AllMatching,
	nullptr);

	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARogueGameModeBase::OnCoinSpawnQueryComplete);
	}
}
void ARogueGameModeBase::OnCoinSpawnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Error, TEXT("Spawn coin EQS query failed"))
		return;
	}

	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);
	
	for (float Counter = 0.f; Counter < MaxCoinsToSpawn; Counter++) {
		int32 Index = FMath::RandRange(0, Locations.Num() - 1);

		GetWorld()->SpawnActor<AActor>(CoinClass, Locations[Index], FRotator::ZeroRotator);
	}
}

void ARogueGameModeBase::SpawnHealthAtStart() {
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
	GetWorld(),
	SpawnHealthQuery,
	this, 
	EEnvQueryRunMode::AllMatching,
	nullptr);

	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARogueGameModeBase::OnHealthSpawnQueryComplete);
	}
}

void ARogueGameModeBase::OnHealthSpawnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Error, TEXT("Spawn health EQS query failed"))
		return;
	}
	
	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);

	for (float Counter = 0.f; Counter < MaxHealthToSpawn; Counter++) {
		int32 Index = FMath::RandRange(0, Locations.Num() - 1);

		GetWorld()->SpawnActor<AActor>(HealthPotionClass, Locations[Index], FRotator::ZeroRotator);
	}
}

void ARogueGameModeBase::RespawnPlayerTimeElapsed(AController* Controller) {
	if (ensure(Controller)) {
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}

void ARogueGameModeBase::KillAllBots() {
	for (TActorIterator<ARogueAICharacter> It(GetWorld()); It; ++It) {
		ARogueAICharacter* Bot = *It;

		URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive()) {
			AttributeComponent->Kill(Bot); // TODO: Maybe pass in player for kill credit?
		}
	}
}

void ARogueGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer) {
	ARogueCharacter* Player = Cast<ARogueCharacter>(VictimActor);
	if (Player) {
		// Remove credits
		URGameplayFunctionLibrary::TakeCredits(Player->GetController(), 10);
		
		// Respawning
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerTimeElapsed", Player->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, PlayerRespawnDelay, false);
	}
}
