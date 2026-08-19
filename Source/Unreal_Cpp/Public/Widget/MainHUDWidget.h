

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

class UStatBarWidget;
class UInventoryWidget;
/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TestInventoryRefresh() const;
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UStatBarWidget>		PlayerStatBar;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UInventoryWidget>	Inventory;
};
