


#include "Framework/SubSystem/PickupFactorySubsystem.h"
#include "Framework/SubSystem/ObjectPoolSubsystem.h"

APickupBase* UPickupFactorySubsystem::SpawnPickup(const UItemDataAsset* InData, const FTransform& InTransForm)
{
	if (!InData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] : UItemDataAsset* InData was nullptr"));
		return nullptr;
	}

	if (!InData->IsLoaded())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] :  data asset &s was not loaded"), *InData->DisplayName.ToString());
		return nullptr;
	}

	return SpawnProcess(InData, InTransForm);

}

void UPickupFactorySubsystem::SpawnPickupAsync(const UItemDataAsset* InData, const FTransform& InTransForm)
{
	if (!InData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] : UItemDataAsset* InData was nullptr"));
		return;
	}

	if (InData->IsLoaded())
	{
		APickupBase* SpawnedWeaponPickUp = SpawnProcess(InData, InTransForm);
		return;
	}

	TWeakObjectPtr<const UItemDataAsset> WeakDataAsset = InData;
	TSharedPtr<FStreamableHandle> AsyncHandle = InData->RequestDataLoad(
		FStreamableDelegate::CreateWeakLambda(
			this,
			[this, WeakDataAsset, InTransForm]()
			{
				if (!WeakDataAsset.IsValid())
				{
					UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] : In AsyncLoading Data asset was invalid"));
					CleanupCompletedHandles();
					return;
				}

				//로딩 시작
				const UItemDataAsset* LoadedAsset = WeakDataAsset.Get();
				APickupBase* Spawned = SpawnProcess(LoadedAsset, InTransForm);
				UE_LOG(LogTemp, Log, TEXT("[UPickupFactorySubsystem] : %s AsyncLoading completed"), *(LoadedAsset->DisplayName).ToString());
				CleanupCompletedHandles();
			}
		)
	);

	if (AsyncHandle.IsValid())
	{
		ActiveStreamableHandles.Add(AsyncHandle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] : AsyncLoad Request was failed"));
	}
}

void UPickupFactorySubsystem::SpawnPickupAsync(UItemDataAsset* InData, const FTransform& InTransForm, FOnpickupSpawned OnSpawned)
{
	if (!InData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] : UItemDataAsset* InData was nullptr"));
		OnSpawned.ExecuteIfBound(nullptr);
		return;
	}

	if (InData->IsLoaded())
	{
		APickupBase* SpawnedWeaponPickUp = SpawnProcess(InData, InTransForm);
		OnSpawned.ExecuteIfBound(SpawnedWeaponPickUp);
		return;
	}

	TWeakObjectPtr<UItemDataAsset> WeakDataAsset = InData;
	TSharedPtr<FStreamableHandle> AsyncHandle = InData->RequestDataLoad(
		FStreamableDelegate::CreateWeakLambda(
			this,
			[this, WeakDataAsset, InTransForm, OnSpawned]()
			{
				if (!WeakDataAsset.IsValid())
				{
					UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] : In AsyncLoading Data asset was invalid"));
					OnSpawned.ExecuteIfBound(nullptr);
					CleanupCompletedHandles();
					return;
				}
				
				//로딩 시작
				UItemDataAsset* LoadedAsset = WeakDataAsset.Get();
				APickupBase* Spawned = SpawnProcess(LoadedAsset, InTransForm);
				OnSpawned.ExecuteIfBound(Spawned);
				UE_LOG(LogTemp, Log, TEXT("[UPickupFactorySubsystem] : %s AsyncLoading completed"), *(LoadedAsset->DisplayName).ToString());
				CleanupCompletedHandles();
			}
		)
	);

	if (AsyncHandle.IsValid())
	{
		ActiveStreamableHandles.Add(AsyncHandle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] : AsyncLoad Request was failed"));
		OnSpawned.ExecuteIfBound(nullptr);
	}
}

void UPickupFactorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[UPickupFactorySubsystem] : Initialized"));
}

void UPickupFactorySubsystem::Deinitialize()
{

	for (TSharedPtr<FStreamableHandle>& Handle : ActiveStreamableHandles)
	{
		if (Handle.IsValid() && Handle->IsActive())
		{
			Handle->CancelHandle();
		}
	}
	ActiveStreamableHandles.Empty();
	UE_LOG(LogTemp, Log, TEXT("[UPickupFactorySubsystem] : Deinitialized"));
	Super::Deinitialize();
}

bool UPickupFactorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer)) return false;

	//const UWorld* World = Cast<UWorld>(Outer);
	//World->IsGameWorld();
	//World->IsEditorWorld();
	//World->WorldType == EWorldType::Game;
	return true;
}

APickupBase* UPickupFactorySubsystem::SpawnProcess(const UItemDataAsset* InData, const FTransform& InTransForm)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] : World was nullptr"));
		return nullptr;
	}

	if (!InData->PickUpActor.Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPickupFactorySubsystem] : pickup actor in %s was nullptr"), *InData->DisplayName.ToString());
		return nullptr;
	}

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!GameInstance) nullptr;

	UObjectPoolSubsystem* SubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>();
	APickupBase* SpawnedWeaponPickUp = SubSystem->Spawn<APickupBase>(InData->PickUpActor.Get(), InTransForm);
	if (SpawnedWeaponPickUp)
	{
		SpawnedWeaponPickUp->Init(InData);
		UE_LOG(LogTemp, Log, TEXT("[UPickupFactorySubsystem] : %s Spawned"), *(InData->DisplayName).ToString());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[UPickupFactorySubsystem] : %s Spawned Failed"), *(InData->DisplayName).ToString());
	}

	return SpawnedWeaponPickUp;
}

void UPickupFactorySubsystem::CleanupCompletedHandles()
{
	// 해당 조건에 맞는 원소를 모두 삭제
	ActiveStreamableHandles.RemoveAll(
		[](const TSharedPtr<FStreamableHandle>& Handle)
		{
			// 핸들이 없거나, 로딩이 끝난 핸들을 대상으로 선정
			return !Handle.IsValid() || Handle->HasLoadCompleted();
		}
	);
}
