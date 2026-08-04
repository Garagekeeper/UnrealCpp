


#include "Framework/SubSystem/ObjectPoolSubsystem.h"
#include "Config/ObjectPoolSettings.h"
#include "Interface/Poolable.h"

AActor* UObjectPoolSubsystem::Spawn(AActor* InType, const FTransform& InTransForm)
{
	if (!IsValid(InType)) return nullptr;
	if (!GetWorld()) return nullptr;
	if (!PoolMap.Find(InType->GetClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("This class can not ppoolable"));
		return nullptr;
	}


	UClass* Target = InType->GetClass();
	AActor* Spawned = nullptr;


	if (PoolMap[Target].ReadyActors.Num() > 0)
	{
		Spawned = PoolMap[Target].ReadyActors.Pop();
		Spawned->SetActorTransform(InTransForm);
	}
	else
	{
		if (PoolableClass.Contains(Target) && GetWorld())
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.Owner = nullptr;
			SpawnParam.ObjectFlags = RF_Transient;

			Spawned = GetWorld()->SpawnActor<AActor>(PoolableClass[Target], InTransForm, SpawnParam);

#if WITH_EDITOR
			if (Spawned)
			{
				Spawned->SetFolderPath(FName("Pool"));
			}
#endif
		}
	}

	if (Spawned)
	{
		if (Spawned->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
		{
			IPoolable::Execute_Onspawn(Spawned);
			PoolMap[Target].ActiveActors.Add(Spawned);
		}
	}

	return Spawned;
}

void UObjectPoolSubsystem::ReturnPool(AActor* InActor)
{
	if (InActor)
	{

		if (InActor->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
		{
			IPoolable::Execute_OnReturn(InActor);
		}

		UClass* Target = InActor->GetClass();
		PoolMap[Target].ActiveActors.Remove(InActor);
		PoolMap[Target].ReadyActors.Add(InActor);
	}
}

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase & Collection)
{
	Super::Initialize(Collection);

	const UObjectPoolSettings* Settings = GetDefault<UObjectPoolSettings>();
	if (Settings)
	{
		//if (Settings->DamagePopupClass.IsNull())
		//{
		//	DamagePopupClass = Settings->DamagePopupClass.LoadSynchronous();
		//}

		for (TSoftClassPtr<AActor> PClass : Settings->PoolableClass)
		{
			CreatePool(PClass);
		}
	}
}

void UObjectPoolSubsystem::CreatePool(TSoftClassPtr<AActor> InActor)
{
	UClass* InClass = InActor.Get();
	if (!PoolMap.Find(InClass))
	{
		
		PoolableClass.FindOrAdd(InClass, InActor.LoadSynchronous());
		PoolMap.FindOrAdd(InClass, FActorPool());
	}
}
