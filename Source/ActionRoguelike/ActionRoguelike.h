// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_STATS_GROUP(TEXT("Rogue_Game"), STATGROUP_ROGUE, STATCAT_ADVANCED);

static void LogOnScreen(UObject* WorldContext, FString Message, FColor Color = FColor::White, float Duration = 5.f) {
	if (!ensure(WorldContext)) {
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World)) {
		return;
	}

	FString NetPrefix = World->IsNetMode(ENetMode::NM_Client) ? "{CLIENT} " : "{SERVER} ";
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Message);
	}
}