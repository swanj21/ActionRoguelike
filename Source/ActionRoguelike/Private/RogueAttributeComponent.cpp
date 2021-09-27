// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeComponent.h"

// Sets default values for this component's properties
URogueAttributeComponent::URogueAttributeComponent() {}

bool URogueAttributeComponent::ApplyHealthChange(float Delta) {
	Health += Delta;

	return true;
}
