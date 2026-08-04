

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamagePopupWidget.generated.h"


class UTextBlock;

DECLARE_DELEGATE(FOnPopupAnimationFinished);

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UDamagePopupWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable)
	void SetDamage(float InDamage);

	UFUNCTION(BlueprintCallable)
	void PlayPopupAnimation();

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;


public:
	FOnPopupAnimationFinished OnPopupAnimationFinished;

protected:
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PopupAnimation;
	
};
