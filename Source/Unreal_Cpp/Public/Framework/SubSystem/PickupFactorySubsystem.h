

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/Item/ItemDataAsset.h"
#include "Engine/StreamableManager.h"
#include "PickupFactorySubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnpickupSpawned, APickupBase*, SpawnedPickup);

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UPickupFactorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Factory|Pickup")
	APickupBase* SpawnPickup(UItemDataAsset* InData, const FTransform& InTransForm);

	UFUNCTION(BlueprintCallable, Category = "Factory|Pickup")
	void SpawnPickupAsync(UItemDataAsset* InData, const FTransform& InTransForm, FOnpickupSpawned OnSpawned);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

private:
	APickupBase* SpawnProcess(UItemDataAsset* InData, const FTransform& InTransForm);
	void CleanupCompletedHandles();

private:
	TArray<TSharedPtr<FStreamableHandle>> ActiveStreamableHandles;

	
	
};
