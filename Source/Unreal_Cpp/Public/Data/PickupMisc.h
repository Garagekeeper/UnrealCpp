

#pragma once

#include "CoreMinimal.h"
#include "Item/PickupBase.h"
#include "Data/Item/ItemDataAsset.h"
#include "PickupMisc.generated.h"

class UMiscItemDataAsset;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API APickupMisc : public APickupBase
{
	GENERATED_BODY()
	
public:
	APickupMisc();
	virtual void Init(UItemDataAsset* asset);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual UMeshComponent* GetMesh() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TWeakObjectPtr<UMiscItemDataAsset> MiscData;
};
