

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DetailInfoWidget.generated.h"


class UTextBlock;
class UImage;
class UItemDataAsset;
class UCanvasPanelSlot;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UDetailInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void DetailOpen(const UItemDataAsset* InData);
	void DetailClose();

	FVector2D ParentPosition;
	inline void SetParentPosition(const FVector2D& InPosition) { ParentPosition = InPosition; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdateLocation();
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UTextBlock> ItemPriceText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UTextBlock> ItemDescriptionText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UImage> ItemIcon;

private:
	bool bTickEnable = false;
	TWeakObjectPtr<UCanvasPanelSlot> CanvasSlot;
	FVector2D ParentPanelPosition;
};
