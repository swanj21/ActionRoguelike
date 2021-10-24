// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "RGameplayFunctionLibrary.h"
#include "RogueAttributeComponent.h"
#include "RogueCharacter.h"
#include "RogueGameplayInterface.h"
#include "RPlayerState.h"
#include "RSaveGame.h"
#include "AI/RogueAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rogue.SpawnBots"), true, TEXT("Enable spawning of bots via a timer"), ECVF_Cheat);

ARogueGameModeBase::ARogueGameModeBase() {
	MinionSpawnTimeInterval = 2.f;
	MaxBotsToSpawn = 10.f;
	MaxCoinsToSpawn = 10.f;
	MaxHealthToSpawn = 3.f;
	PlayerRespawnDelay = 2.f;
	RequiredObjectDistance = 1000.f;

	PlayerStateClass = ARPlayerState::StaticClass();

	SlotName = "SaveGame_01";
}

void ARogueGameModeBase::StartPlay() {
	Super::StartPlay();

	UE_LOG(LogTemp, Warning, TEXT("Spawning items"))
	SpawnHealthAtStart();
	SpawnCoinsAtStart();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ARogueGameModeBase::SpawnBotTimerElapsed, MinionSpawnTimeInterval, true);
}

void ARogueGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

/**
 * Save the game
 */
void ARogueGameModeBase::WriteSaveGame() {
	// Iterate all player states, we don't have proper IDs to match yet to save multiplayer data(requires Steam or EOS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++) {
		ARPlayerState* PlayerState = Cast<ARPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState) {
			PlayerState->SavePlayerState(CurrentSaveGame);
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	// Iterate all actors in the world
	for (FActorIterator It(GetWorld()); It; ++It) {
		AActor* Actor = *It;
		// Only interested in our gameplay actors
		if (!Actor->Implements<URogueGameplayInterface>()) {
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemoryWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

/**
 * Load the previously saved game
 */
void ARogueGameModeBase::LoadSaveGame() {
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) {
		// Find the save game file
		CurrentSaveGame = Cast<URSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame data."))
			return;
		}
		// Find information about the saved actors and update the transforms
		for (FActorIterator It(GetWorld()); It; ++It) {
			AActor* Actor = *It;
			// Only interested in our gameplay actors
			if (!Actor->Implements<URogueGameplayInterface>()) {
				continue;
			}
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors) {
				if (ActorData.ActorName == Actor->GetName()) {
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemoryReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(Archive);

					IRogueGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame data."))
	} else {
		CurrentSaveGame = Cast<URSaveGame>(UGameplayStatics::CreateSaveGameObject(URSaveGame::StaticClass()));
		UE_LOG(LogTemp, Warning, TEXT("Created new SaveGame data."))
	}
}

void ARogueGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) {
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	ARPlayerState* PlayerState = NewPlayer->GetPlayerState<ARPlayerState>();
	if (PlayerState) {
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
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

	// Keeping track of the locations we have already used
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	while(SpawnCounter < MaxCoinsToSpawn && Locations.Num() > 0) {
		// Pick a random location
		int32 Index = FMath::RandRange(0, Locations.Num() - 1);

		FVector ChosenLocation = Locations[Index];
		// Remove from the array so we don't pick it again
		Locations.RemoveAt(Index);

		// Check to see if it is at a minimum distance from any other existing locations
		bool bIsValidLocation = true;
		for (FVector UsedLocation : UsedLocations) {
			float DistanceBetween = (ChosenLocation - UsedLocation).Size();

			if (DistanceBetween < RequiredObjectDistance) {
				bIsValidLocation = false;
				break;
			}
		}

		if (!bIsValidLocation) {
			continue;
		}
		GetWorld()->SpawnActor<AActor>(CoinClass, ChosenLocation, FRotator::ZeroRotator);

		UsedLocations.Add(ChosenLocation);
		SpawnCounter++;
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
