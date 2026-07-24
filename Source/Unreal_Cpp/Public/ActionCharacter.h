// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputactionValue.h"
#include "../Interface/StatHolder.h"
#include "ActionCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UUserWidget;
class UProgressBar;
class UStatComponent;

UCLASS()
class UNREAL_CPP_API AActionCharacter : public ACharacter, public IStatHolder
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	//virtual UStatComponent* GetStatComponent_Implementation() const override;
	virtual UStatComponent* GetStatComponent() const override;

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

private:
	// Consume Stamina per sec when Sprinting
	void ConsumeSprintStamina(float DeltaTime);

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
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> StaminaWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UUserWidget> CreatedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WalkSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SprintSpeed = 1200.0f;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Stat|Stamina")
	float RollStaminaUsage = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Stamina")
	float SprintStaminaUsagePerSec = 5.0f;

	//// DEPRECATED 스테미너 자동 회복 시 초당 회복량
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Stamina")
	//float AutoStaminaRecoveryPerSec = 10.0f;

	// 스테미너가 자동 회복 될 때 타이머 틱당 회복량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Stamina")
	float AutoStaminaRecoveryPerTick = 1.0f;

	// 스테미너가 자동 회복 될 때 타이머 틱 주기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Stamina")
	float AutoStaminaRecoveryInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Stamina")
	float StaminaAutoRecoveryCoolTime = 3.0f;

	
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent> StatComponent = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;

	bool bRun = false;

};
