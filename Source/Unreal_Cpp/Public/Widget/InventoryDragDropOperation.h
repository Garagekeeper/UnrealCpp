

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Data/Item/ItemDataAsset.h"
#include "InventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<const UItemDataAsset> ItemData;

	UPROPERTY(BlueprintReadOnly)
	int32 Index;
	
};
