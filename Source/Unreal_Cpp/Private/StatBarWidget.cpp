// Fill out your copyright notice in the Description page of Project Settings.


#include "StatBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "../Interface/StatHolder.h"
#include "StatComponent.h"
#include "BarWidget.h"

void UStatBarWidget::NativeConstruct()
{
	
	AActor* Player = GetOwningPlayerPawn();
	IStatHolder* Holder = Cast<IStatHolder>(Player);
	if (Holder)
	{
		Holder->GetStatComponent()->OnStaminaChange.AddDynamic(StaminBar, &UBarWidget::UpdateBar);
		Holder->GetStatComponent()->OnHealthChange.AddDynamic(HpBar, &UBarWidget::UpdateBar);
	}

}

void UStatBarWidget::NativeDestruct()
{

}

void UStatBarWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{

}
