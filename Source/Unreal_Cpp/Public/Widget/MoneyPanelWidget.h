

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoneyPanelWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UMoneyPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMoney(int32 InMoney);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UTextBlock> MoneyText;
};
