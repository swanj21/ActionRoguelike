// Fill out your copyright notice in the Description page of Project Settings.


#include "RWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void URWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Project world location to screen location
	FVector2D ScreenPosition;
	if (!IsValid(AttachedActor)) {
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor is no longer valid, removing health widget"))

		return;
	}
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition)) {
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale;

		if (ParentSizeBox) { ParentSizeBox->SetRenderTranslation(ScreenPosition); }
	}
}
