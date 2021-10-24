// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RSaveGame.generated.h"

USTRUCT()
struct FActorSaveData {
	GENERATED_BODY()

public:
	// Which actor this belongs to
	UPROPERTY()
	FString ActorName;

	// Location information
	UPROPERTY()
	FTransform Transform;

	// Deserialized data in bytes
	UPROPERTY()
	TArray<uint8> ByteData;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
