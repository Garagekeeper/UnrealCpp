
#pragma once


#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StreamableManager.h"
#include "Item/PickupBase.h"
#include "ItemDataAsset.generated.h"

/**
 *
 */
UCLASS(Abstract, BlueprintType)
class UNREAL_CPP_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	TSharedPtr<FStreamableHandle> RequestDataLoad(FStreamableDelegate InDelegate) const;
	virtual bool IsLoaded() const;

protected:
	virtual void OnAsyncRequest(TArray<FSoftObjectPath>& InOutArray) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base|Data")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base|Data")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base|Spawn")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base|Data")
	int32 Price = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base|Data")
	int32 MaxStackCnt = 1;

	// 스폰시의 오프셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base|Spawn")
	FVector SpwanLocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base|Spawn")
	TSoftClassPtr<APickupBase> PickUpActor;


};
