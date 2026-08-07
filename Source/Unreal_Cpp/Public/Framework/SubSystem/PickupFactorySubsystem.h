

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/Item/ItemDataAsset.h"
#include "PickupFactorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UPickupFactorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	APickupBase* SpawnPickup(UItemDataAsset* InData, FTransform InTransForm);

	
	
	
};
