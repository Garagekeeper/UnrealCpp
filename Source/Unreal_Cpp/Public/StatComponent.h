// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interface/StaminaInterface.h"
#include "../Interface/HealthInterface.h"
#include "StatComponent.generated.h"

struct FAutoRecoveryData
{
	// 회복 발동까지 걸릴 시간
	float CoolTime = 3.0f;

	// 틱 주기
	float TickInterval = 0.1f;

	// 틱당 회복량
	float RecoveryPerTick = 1.0f;

	FAutoRecoveryData() : CoolTime(1.0f), TickInterval(1.0), RecoveryPerTick(1.0) {}
	FAutoRecoveryData(float InCoolTime, float InTickInterval, float InRecoveryPerTick)
		: CoolTime(InCoolTime), TickInterval(InTickInterval), RecoveryPerTick(InRecoveryPerTick) {}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_CPP_API UStatComponent : public UActorComponent, public IStaminaInterface, public IHealthInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

	void InitializeStat(FAutoRecoveryData& Indata);

	virtual float GetCurrentStamina_Implementation() const override;
	virtual bool ConsumeStamina_Implementation(float InAmount) override;
	virtual void RecoveryStamina_Implementation(float InAmount) override;
	virtual float GetMaxStamina_Implementation() const override;

	virtual float GetCurrentHealth_Implementation() const override;
	virtual bool ApplyDamage_Implementation(float InAmount) override;
	virtual void ApplyHeal_Implementation(float InAmount) override;
	virtual float GetMaxHealth_Implementation() const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//DEPRECATED : 타이머로 대체
	//void StaminaAutoRecovery(float DeltaTime);

private:
	void StaminaAutoRecoveryPerTick();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxStamina = 100.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 100.0f;

private:
	//DEPRECATED
	float StaminaAutoRecoverySec = 3.0f;

	FTimerHandle SprintStaminaConsumeTimer;
	// 자동회복 처리 타이머
	FTimerHandle AutoRecorveryStaminaTimer;
	
	// 스테미너 자동 회복용 데이터
	FAutoRecoveryData StaminaRecoveryData;
};
