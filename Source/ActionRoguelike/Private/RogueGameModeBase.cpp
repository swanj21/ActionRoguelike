// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "RogueAttributeComponent.h"
#include "AI/RogueAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ARogueGameModeBase::ARogueGameModeBase() {
	SpawnTimeInterval = 2.f;
}

void ARogueGameModeBase::StartPlay() {
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ARogueGameModeBase::SpawnBotTimerElapsed, SpawnTimeInterval, true);
}

void ARogueGameModeBase::SpawnBotTimerElapsed() {
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		GetWorld(),
		SpawnBotQuery,
		this, 
		EEnvQueryRunMode::RandomBest5Pct,
		nullptr);

	int32 CurAliveBots = 0;
	for (TActorIterator<ARogueAICharacter> It(GetWorld()); It; ++It) {
		ARogueAICharacter* Bot = *It;

		URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive()) {
			CurAliveBots++;
		}
	}

	float MaxBotCount = 10.f;
	if (DifficultyCurve) {
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i bots alive and %f bots max"), CurAliveBots, MaxBotCount)

	if (CurAliveBots >= MaxBotCount) {
		UE_LOG(LogTemp, Log, TEXT("At max capacity, skipping spawn"))
		return;
	}
	
	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARogueGameModeBase::OnQueryComplete);
	}
}

void ARogueGameModeBase::OnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
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

void ARogueGameModeBase::KillAllBots() {
	for (TActorIterator<ARogueAICharacter> It(GetWorld()); It; ++It) {
		ARogueAICharacter* Bot = *It;

		URogueAttributeComponent* AttributeComponent = URogueAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive()) {
			AttributeComponent->Kill(Bot); // TODO: Maybe pass in player for kill credit?
		}
	}
}
