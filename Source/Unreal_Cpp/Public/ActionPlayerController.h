// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputactionValue.h"
#include "ActionPlayerController.generated.h"


class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API AActionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ViewPitchMax = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ViewPitchMin = -40;

private:
	void OnLookAction(const FInputActionValue& Value);

private:
	// 나의 입력 우선순위를 제일 높게 하겠다. (숫자 낮으면 높은거임)
	int32 GameInputPriority = 1;
};
