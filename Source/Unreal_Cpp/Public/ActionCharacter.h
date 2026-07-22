// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputactionValue.h"
#include "../Interface/StaminaInterface.h"
#include "ActionCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UUserWidget;
class UProgressBar;

UCLASS()
class UNREAL_CPP_API AActionCharacter : public ACharacter, public IStaminaInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();
	virtual float GetCurrentStamina_Implementation() const override;
	virtual bool ConsumeStamina_Implementation(float InAmount) override;
	virtual void RecoveryStamina_Implementation(float InAmount) override;
	virtual float GetMaxStamina_Implementation() const override;

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
	void OnSprintStart();
	void OnSprintEnd();
	void OnRollAction(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Test;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Roll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> StaminaWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UUserWidget> CreatedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WalkSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SprintSpeed = 1200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentStamina = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RollStaminaUsage = 30.0f;

	bool bRun = false;
	float NonuseElapsed = 0;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;


private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;
};
