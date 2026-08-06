


#include "Framework/SubSystem/ObjectPoolSubsystem.h"
#include "Config/ObjectPoolSettings.h"
#include "Interface/Poolable.h"

//AActor* UObjectPoolSubsystem::Spawn(AActor* InType, const FTransform& InTransForm)
AActor* UObjectPoolSubsystem::Spawn(TSubclassOf<AActor> InType, const FTransform& InTransForm)
{
	if (!InType) return nullptr;
	if (!GetWorld()) return nullptr;
	//if (!PoolMap.Find(InType))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("This class can not ppoolable"));
	//	return nullptr;
	//}


	//UClass* Target = InType->GetClass();

	AActor* Spawned = nullptr;

	while (PoolMap.Find(InType) && PoolMap[InType].ReadyActors.Num() > 0)
	{
		AActor* Candidate = PoolMap[InType].ReadyActors.Pop();
		if (IsValid(Candidate))
		{
			Spawned = Candidate;
			Spawned->SetActorTransform(InTransForm, false, nullptr, ETeleportType::TeleportPhysics);
			break;
		}
	}

	if (!Spawned)
	{
		switch (PoolMap[InType].Policy)
		{
			case EObjectPoolPolicy::ReUseOldest:
				if (PoolMap[InType].ActiveActors.Num() >= PoolMap[InType].MaxSize)
				{
					//TODO Find Oldest
					TObjectPtr<AActor> Oldest = nullptr;
					double MinVal = GetWorld()->GetTimeSeconds();
					for (auto& [Key, Val] : PoolMap[InType].ActiveActors)
					{
						if (Val < MinVal)
						{
							Oldest = Key;
							MinVal = Val;
						}
					}

					Spawned = Oldest;
					ReturnPool(Spawned);
					Spawned->SetActorTransform(InTransForm, false, nullptr, ETeleportType::TeleportPhysics);
					UE_LOG(LogTemp, Log, TEXT("%s, Reused by ReuseOldest"), *(InType->GetName()));
				}
				else
				{
					SpawnActorToWorld(InType, Spawned, InTransForm);
				}
				break;

			case EObjectPoolPolicy::DoNotSpawn:
				if (PoolMap[InType].ActiveActors.Num() < PoolMap[InType].MaxSize)
				{
					SpawnActorToWorld(InType, Spawned, InTransForm);
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("%s, Not Spawned by DoNotSpawn"), *(InType->GetName()));
				}

				break;
			case EObjectPoolPolicy::ForceGrow:
				if (PoolMap[InType].ActiveActors.Num() >= PoolMap[InType].MaxSize)
					UE_LOG(LogTemp, Log, TEXT("%s, Spawned by ForceGrow"), *(InType->GetName()));
				SpawnActorToWorld(InType, Spawned, InTransForm);
				break;
			default:
				break;
		}

	}

	if (Spawned)
	{
		if (Spawned->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
		{
			IPoolable::Execute_Onspawn(Spawned);
		}

		else
		{
			Spawned->SetActorHiddenInGame(false);
			Spawned->SetActorTickEnabled(true);
			Spawned->SetActorEnableCollision(true);
		}

		PoolMap[InType].ActiveActors.Add(Spawned, GetWorld()->GetTimeSeconds());
	}


	return Spawned;
}

void UObjectPoolSubsystem::SpawnActorToWorld(TSubclassOf<AActor> Target, AActor*& Spawned, const FTransform& InTransForm)
{
	if (PoolMap.Contains(Target) && GetWorld())
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = nullptr;
		SpawnParam.ObjectFlags = RF_Transient;

		Spawned = GetWorld()->SpawnActor<AActor>(Target, InTransForm, SpawnParam);

#if WITH_EDITOR
		if (Spawned)
		{
			Spawned->SetFolderPath(FName("PoolObjects"));
		}
#endif
	}
}

void UObjectPoolSubsystem::ReturnPool(AActor* InActor)
{
	if (!InActor) return;
	UClass* Target = InActor->GetClass();

	if (!PoolMap.Find(Target)) return;
	if (InActor->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
	{
		IPoolable::Execute_OnReturn(InActor);
	}
	else
	{
		InActor->SetActorHiddenInGame(true);
		InActor->SetActorTickEnabled(false);
		InActor->SetActorEnableCollision(false);
	}

	PoolMap[Target].ActiveActors.Remove(InActor);
	PoolMap[Target].ReadyActors.Add(InActor);

}

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UObjectPoolSettings* Settings = GetDefault<UObjectPoolSettings>();
	if (Settings)
	{
		for (const TSoftObjectPtr<UObjectPoolDataAsset>& DataAsset : Settings->PoolableDataAsset)
		{
			if (DataAsset.IsNull()) continue;
			TObjectPtr<UObjectPoolDataAsset> LoadedDataAsset = DataAsset.LoadSynchronous();
			CreatePool(LoadedDataAsset);
		}
	}
}

void UObjectPoolSubsystem::Deinitialize()
{
	ClearAllPools();
	Super::Deinitialize();
}

void UObjectPoolSubsystem::WarmUp(TSubclassOf<AActor> InClass)
{
	if (FActorPool* Pool = PoolMap.Find(InClass))
	{
		FTransform Init(FVector::DownVector * 10000.0f);
		TArray<AActor*> SpawnedArray;
		SpawnedArray.Reserve(Pool->InitialSize);

		AActor* Spawned = nullptr;

		for (int i = 0; i < Pool->InitialSize; i++)
		{
			SpawnActorToWorld(InClass, Spawned, Init);
			SpawnedArray.Add(Spawned);
		}

		for (auto SpawnedActor : SpawnedArray)
		{
			ReturnPool(SpawnedActor);
		}

	}
}


void UObjectPoolSubsystem::WarmUpAll()
{
	for (auto& [Key, _] : PoolMap)
	{
		WarmUp(Key);
	}
}

void UObjectPoolSubsystem::ClearPool(TSubclassOf<AActor> InClass)
{
	if (!InClass) return;
	if (FActorPool* Pool = PoolMap.Find(InClass))
	{
		for (AActor* Actor : Pool->ReadyActors)
		{
			if (IsValid(Actor)) Actor->Destroy();
		}
		Pool->ReadyActors.Empty();
		for (auto& [Key, _] : Pool->ActiveActors)
		{
			if (IsValid(Key)) Key->Destroy();
		}
		Pool->ActiveActors.Empty();
		PoolMap.Remove(InClass);
	}
}

void UObjectPoolSubsystem::ClearAllPools()
{
	TArray<TSubclassOf<AActor>> Keys;
	PoolMap.GetKeys(Keys);
	for (auto& Key : Keys)
	{
		ClearPool(Key);
	}
	PoolMap.Empty();
}

void UObjectPoolSubsystem::CreatePool(const UObjectPoolDataAsset* LoadedDataAsset)
{

	TSubclassOf<AActor> InClass = LoadedDataAsset->ActorClass.LoadSynchronous();

	if (PoolMap.Find(InClass))
	{
		ClearPool(InClass);
	}

	PoolMap.FindOrAdd(InClass,
		FActorPool(LoadedDataAsset->InitialSize, LoadedDataAsset->MaxSize, LoadedDataAsset->PoolPolicy));

}

bool UObjectPoolSubsystem::RegisterPoolDataAsset(const UObjectPoolDataAsset* InDataAsset, bool bWarmUp)
{
	if (!InDataAsset) return false;
	if (InDataAsset->ActorClass.IsNull()) return false;

	CreatePool(InDataAsset);

	if (bWarmUp)
		WarmUp(InDataAsset->ActorClass.Get());


	return true;
}

bool UObjectPoolSubsystem::UnregisterPoolDataAsset(const UObjectPoolDataAsset* InDataAsset)
{
	if (!InDataAsset) return false;
	if (InDataAsset->ActorClass.IsNull()) return false;

	ClearPool(InDataAsset->ActorClass.LoadSynchronous());

	return true;
}
