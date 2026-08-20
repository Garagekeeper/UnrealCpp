// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputactionValue.h"
#include "Interface/StatHolder.h"
#include "Interface/WeaponUserInterface.h"
#include "Interface/InventoryUserInterface.h"
#include "ActionCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
//class UStatComponent;
class UAnimNotifyState_SectionJump;
class AWeaponActor;
class AActionHUD;

UCLASS()
class UNREAL_CPP_API AActionCharacter : 
	public ACharacter, public IStatHolder, public IWeaponUserInterface, public IInventoryUserInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	virtual void EquipWeapon_Implementation(const UWeaponDataAsset* InWeaponData) override;

	// Gettet Setter
	// virtual UStatComponent* GetStatComponent_Implementation() const override;
	virtual UStatComponent* GetStatComponent() const override;
	virtual FOnWeaponAttackStateChanged& GetWeaponAttackStateChangedDelegate() override
	{
		return OnWeaponAttackStateChaned;
	};

	virtual UInventoryComponent* GetInventoryComponent() const override;
	UFUNCTION(BlueprintCallable)
	virtual bool ExecuteInventoryCommand(const FInventoryCommand& Command, FCommandResult& OutResult) const override;


	// 이벤트 함수
	virtual void OnWeaponAttackState(bool bEnable) override;

	void SetSectionJumpNotify(UAnimNotifyState_SectionJump* InSectionJumpNotify);
	void UpdateAttackState(bool Inval);
	void OnAreaAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void OnTestAction(const FInputActionValue& Value);
	void OnInvenAction(const FInputActionValue& Value);
	void OnMoveAction(const FInputActionValue& Value);
	void OnSprintStartAction(const FInputActionValue& Value);
	void OnSprintExitAction(const FInputActionValue& Value);
	void OnSprintStart();
	void OnSprintEnd();
	void OnAttackAction();
	void OnRollAction(const FInputActionValue& Value);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	// Consume Stamina per sec when Sprinting
	void ConsumeSprintStamina(float DeltaTime);
	void SectionJumpForCombo();
	void SpawnWeaponActor();
	
public:
	FOnWeaponAttackStateChanged OnWeaponAttackStateChaned;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Test;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Inven;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Roll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Move")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Move")
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

	// 공격시 소비되는 스테미나
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Attack")
	float AttackStaminaUsage = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TWeakObjectPtr<AWeaponActor> CurrentWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<const UWeaponDataAsset> CurrentWeaponData = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<AWeaponActor> BaseWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<const UWeaponDataAsset> BaseWeaponData = nullptr;

	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent> StatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InvenComponent = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;

	TWeakObjectPtr<AActionHUD> ActionHUD = nullptr;

	// 발생한 콤보 노티파이를 저장해 놓는 변수
	TWeakObjectPtr<UAnimNotifyState_SectionJump> SectionJumpNotify = nullptr;
	// 현재 콤포가 가능한지 확인하기 위한 변수
	bool bComboReady = false;

	UPROPERTY(VisibleAnywhere)
	bool bCanAttack = true;

	bool bRun = false;
};
