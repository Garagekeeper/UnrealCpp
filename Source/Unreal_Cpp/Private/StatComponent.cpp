// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UStatComponent::InitializeStat(FAutoRecoveryData& InData)
{
	StaminaRecoveryData = InData;
}

float UStatComponent::GetCurrentStamina_Implementation() const
{
	return CurrentStamina;
}

bool UStatComponent::ConsumeStamina_Implementation(float InAmount)
{
	bool bResult = false;
	if (CurrentStamina >= InAmount)
	{
		CurrentStamina -= InAmount;
		bResult = true;

		//타이머로 대체
		//StaminaAutoRecoverySec = StaminaAutoRecoveryCoolTime;

		//UWorld* World = GetWorld();
		//FTimerManager& TimerManager = World->GetTimerManager();

		//// 내가 만드 똥
		//TimerManager.ClearTimer(AutoRecorveryStaminaTimer);
		//TimerManager.ClearTimer(AutoRecorveryStamina);

		//StaminaAutoTimer();


		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();

		// 같은 핸들로 여러번 실행하면 덮어쓰기임
		TimerManager.SetTimer(
			AutoRecorveryStaminaTimer,
			this,
			&UStatComponent::StaminaAutoRecoveryPerTick,
			StaminaRecoveryData.TickInterval,
			true,
			StaminaRecoveryData.CoolTime
		);

		OnStaminaChange.Broadcast(CurrentStamina, MaxStamina);

		if (FMath::IsNearlyZero(CurrentStamina, 0.1))
		{
			OnStaminaEmpty.Broadcast();
		}

	}

	CurrentStamina = FMath::Clamp(CurrentStamina, 0, MaxStamina);
	//UE_LOG(LogTemp, Log, TEXT("현재 Stamina : %.1f"), CurrentStamina);
	return bResult;
}

void UStatComponent::StaminaAutoRecoveryPerTick()
{
	if (!FMath::IsNearlyEqual(CurrentStamina, MaxStamina))
		IStaminaInterface::Execute_RecoveryStamina(this, StaminaRecoveryData.RecoveryPerTick);
	else
	{
		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(AutoRecorveryStaminaTimer);
	}
}

void UStatComponent::RecoveryStamina_Implementation(float InAmount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + InAmount, 0, MaxStamina);

	if (CurrentStamina >= MaxStamina)
	{
		UWorld* World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(AutoRecorveryStaminaTimer);
	}
	//UE_LOG(LogTemp, Log, TEXT("현재 Stamina : %.1f"), CurrentStamina);
}

float UStatComponent::GetMaxStamina_Implementation() const
{
	return FMath::IsNearlyZero(MaxStamina) ? 100.0f : MaxStamina;
}

float UStatComponent::GetCurrentHealth_Implementation() const
{
	return CurrentHealth;
}

void UStatComponent::ApplyDamage_Implementation(float InAmount)
{
	CurrentHealth = CurrentHealth - InAmount;
	if (CurrentHealth < 0.0f)
	{
		CurrentHealth = 0;
		OnDeath.Broadcast();
	}

	OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
}

void UStatComponent::ApplyHeal_Implementation(float InAmount)
{
	CurrentHealth = FMath::Min(CurrentHealth + InAmount, MaxHealth);
	OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
}

float UStatComponent::GetMaxHealth_Implementation() const
{
	return FMath::IsNearlyZero(MaxHealth) ? 100 : MaxHealth;
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentStamina = MaxStamina;
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



//void AActionCharacter::StaminaAutoRecovery(float DeltaTime)
//{
//
//	if (!FMath::IsNearlyEqual(CurrentStamina, MaxStamina))
//		IStaminaInterface::Execute_RecoveryStamina(this, 0.1f * AutoStaminaRecoveryPerSec);
//}

