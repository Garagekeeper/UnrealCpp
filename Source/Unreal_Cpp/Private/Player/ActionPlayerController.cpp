// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"

void AActionPlayerController::OnInventoryOpenClose(bool bInventoryOpen, UUserWidget* InFocusWidget)
{
	//FInputModeGameOnly
	//FInputModeUIOnly
	//FInputModeGameAndUI

	if (bInventoryOpen)
	{
		FInputModeUIOnly InputModeUI;
		InputModeUI.SetWidgetToFocus(InFocusWidget->TakeWidget());

		SetInputMode(InputModeUI);
		SetShowMouseCursor(true);
	}
	else
	{
		FInputModeGameOnly InputModeGame;
		SetInputMode(InputModeGame);
		SetShowMouseCursor(false);
	}

}

void AActionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* SubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (SubSystem && DefaultMappingContext)
	{
		SubSystem->AddMappingContext(DefaultMappingContext, GameInputPriority);
	}
}

void AActionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AActionPlayerController::OnLookAction);
	}

	PlayerCameraManager->ViewPitchMin = ViewPitchMin;
	PlayerCameraManager->ViewPitchMax = ViewPitchMax;

}

void AActionPlayerController::OnLookAction(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	AddYawInput(LookAxis.X);
	AddPitchInput(LookAxis.Y);
}
