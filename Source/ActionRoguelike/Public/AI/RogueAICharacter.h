// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARogueAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueAICharacter();

protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class URogueAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effects")
	FName TimeToHitParamName;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUserWidget> HealthBarWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	class URWorldUserWidget* ActiveHealthBar;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category="Health")
	void OnHealthChanged(AActor* InstigatorActor, class URogueAttributeComponent* OwningComponent, float NewHealth, float Delta);

	UFUNCTION(BlueprintCallable, Category="AI")
	void SetTargetActor(AActor* NewTarget);

	// CREDITS

	UPROPERTY(EditAnywhere, Category="Credits")
	float CreditsToGive;

};
