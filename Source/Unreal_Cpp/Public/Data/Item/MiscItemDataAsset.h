

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/ItemDataAsset.h"
#include "MiscItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UMiscItemDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
public:
	virtual bool IsLoaded() const;

protected:
	virtual void OnAsyncRequest(TArray<FSoftObjectPath>& InOutArray) const;

public:
	// Mesh Of Item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc|Appearance")
	TSoftObjectPtr<UStaticMesh> Mesh;

	
};
