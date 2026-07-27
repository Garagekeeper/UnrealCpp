// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StatBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/StatHolder.h"
#include "Component/StatComponent.h"
#include "Widget/BarWidget.h"

void UStatBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AActor* Player = GetOwningPlayerPawn();
	IStatHolder* Holder = Cast<IStatHolder>(Player);
	if (Holder)
	{
		UStatComponent* Stat = Holder->GetStatComponent();
		if (Stat)
		{
			Stat->OnStaminaChange.AddDynamic(StaminBar, &UBarWidget::UpdateBar);
			Stat->OnHealthChange.AddDynamic(HpBar, &UBarWidget::UpdateBar);
		}
	}

}

void UStatBarWidget::NativeDestruct()
{

}

void UStatBarWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{

}
