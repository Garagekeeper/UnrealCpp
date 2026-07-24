// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BarWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateBar(float InCurrent, float InMax);
	void UpdateBarTarget(float InCurrent);
	void UpdateValueText(float InValue);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Value;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentValue = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText CurrentText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float InterpSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BarColor;


	// 액터의 Construction Script 역할을 하는 핵심 함수
	virtual void SynchronizeProperties() override;
	
};
