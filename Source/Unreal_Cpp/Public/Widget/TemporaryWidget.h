

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemporaryWidget.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UTemporaryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetVisual(UTexture2D* InIcon, int32 InCount);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UTextBlock> CountText;
	
};
