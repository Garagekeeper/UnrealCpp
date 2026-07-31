// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UBarWidget::UpdateBar(float InCurrent, float InMax)
{
	InMax = FMath::Max(0.0001f, InMax);
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

#if WITH_EDITOR
void UBarWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName TargetPropertyName = (PropertyChangedEvent.Property != nullptr) ?
		PropertyChangedEvent.Property->GetFName() : NAME_None;

	// 가져온 프로퍼티 이름이 실제 클래스의 멤버 변수와 동일한 이름인지 확인
	if (TargetPropertyName == GET_MEMBER_NAME_CHECKED(UBarWidget, FillColor))
	{
		BackGroundColor = FillColor;
		BackGroundColor.A = 0.2f;
	}
}
#endif

void UBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Bar->SetFillColorAndOpacity(FillColor);

	FProgressBarStyle Style = Bar->GetWidgetStyle();
	Style.BackgroundImage.TintColor = BackGroundColor;
	Bar->SetWidgetStyle(Style);
}

void UBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	float FinalVal = FMath::FInterpTo(Bar->GetPercent(), CurrentValue, InDeltaTime, InterpSpeed);
	Bar->SetPercent(FinalVal);
	//UE_LOG(LogTemp, Log, TEXT("%f"), FinalVal);
}

//void UBarWidget::SynchronizeProperties()
//{
//	Super::SynchronizeProperties();
//
//	if (Bar)
//	{
//		Bar->SetFillColorAndOpacity(FillColor);
//	}
//}

