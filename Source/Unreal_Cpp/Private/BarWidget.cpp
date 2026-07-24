// Fill out your copyright notice in the Description page of Project Settings.


#include "BarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UBarWidget::UpdateBar(float InCurrent, float InMax)
{
	UpdateBarTarget(InCurrent / InMax);
	UpdateValueText(InCurrent);
}

void UBarWidget::UpdateBarTarget(float InCurrent)
{
	CurrentValue = InCurrent;
}

void UBarWidget::UpdateValueText(float InValue)
{

	CurrentText = FText::AsNumber(round(InValue));
	Value->SetText(CurrentText);
}

void UBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	float FinalVal = FMath::FInterpTo(Bar->GetPercent(), CurrentValue, InDeltaTime, InterpSpeed);
	Bar->SetPercent(FinalVal);
	//UE_LOG(LogTemp, Log, TEXT("%f"), FinalVal);
}

void UBarWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (Bar)
	{
		Bar->SetFillColorAndOpacity(BarColor);
	}
}

