// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, FString Message, FColor Color = FColor::White, float DUration = 5.f) {
	if (!ensure(WorldContext)) {
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World)) {
		return;
	}

	FString NetPrefix = World->IsNetMode(ENetMode::NM_Client) ? "{CLIENT} " : "{SERVER} ";
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, DUration, Color, NetPrefix + Message);
	}
}