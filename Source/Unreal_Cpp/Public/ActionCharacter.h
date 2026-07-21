// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputactionValue.h"
#include "ActionCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UNREAL_CPP_API AActionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void OnTestAction(const FInputActionValue& Value);
	void OnMoveAction(const FInputActionValue& Value);
	void OnSprintStartAction(const FInputActionValue& Value);
	void OnSprintExitAction(const FInputActionValue& Value);
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Test;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Sprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SprintSpeed;
};
