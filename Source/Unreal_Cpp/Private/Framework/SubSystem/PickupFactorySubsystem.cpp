


#include "Framework/SubSystem/PickupFactorySubsystem.h"
#include "Framework/SubSystem/ObjectPoolSubsystem.h"

APickupBase* UPickupFactorySubsystem::SpawnPickup(UItemDataAsset* InData, FTransform InTransForm)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) nullptr;

	UObjectPoolSubsystem* SubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>();
	APickupBase* SpawnedWeaponPickUp = SubSystem->Spawn<APickupBase>(InData->PickUpActor.Get(), InTransForm);
	if (SpawnedWeaponPickUp)
	{
		SpawnedWeaponPickUp->Init(InData);
		UE_LOG(LogTemp, Log, TEXT("%s Spawned"), *(InData->DisplayName).ToString());
	}

	return SpawnedWeaponPickUp;
}
