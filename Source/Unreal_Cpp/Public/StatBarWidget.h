// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBarWidget.generated.h"

class UImage;
class UBarWidget;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	
protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//TObjectPtr<UProgressBar> StaminaBar;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//TObjectPtr<UProgressBar> HealthBar;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//TObjectPtr<UTextBlock> StaminaText;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//TObjectPtr<UTextBlock> HealthText;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	//TObjectPtr<UImage> TestImage;
	//
	//// 런타임에 임시로 생성된다는 애라는 뜻
	//UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
	//TObjectPtr<UWidgetAnimation> TestAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBarWidget> HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBarWidget> StaminBar;

};
