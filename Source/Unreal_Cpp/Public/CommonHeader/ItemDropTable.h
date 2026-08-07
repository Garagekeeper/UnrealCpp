

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/WeaponDataAsset.h"
#include "ItemDropTable.generated.h"

USTRUCT(BlueprintType)
struct FItemDropTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemDataAsset> DropItemAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1"))
	float DropRate = 0.5f;
};