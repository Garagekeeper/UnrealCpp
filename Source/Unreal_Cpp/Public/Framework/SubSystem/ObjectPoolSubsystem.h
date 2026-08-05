

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/ObjectPoolDataAsset.h"
#include "CommonHeader/ObjectPoolEnums.h"
#include "ObjectPoolSubsystem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FActorPool
{
	GENERATED_BODY()
	FActorPool() {};
	FActorPool(int32 InInitialSize, int32 InMaxSize, EObjectPoolPolicy InPolicy) 
		:InitialSize(InInitialSize), MaxSize(InMaxSize), Policy(InPolicy){}

	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> ReadyActors;

	// 실제 사용 중인 액터들
	UPROPERTY(Transient)
	TMap<TObjectPtr<AActor>, double> ActiveActors;


	UPROPERTY(Transient)
	int32 InitialSize = 0;

	UPROPERTY(Transient)
	int32 MaxSize = 0;

	UPROPERTY(Transient)
	EObjectPoolPolicy Policy = EObjectPoolPolicy::ForceGrow;

};

UCLASS()
class UNREAL_CPP_API UObjectPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//template<typename T>
	UFUNCTION(BlueprintCallable)
	//AActor* Spawn(AActor* InType, const FTransform& InTransForm);
	AActor* Spawn(TSubclassOf<AActor> InType, const FTransform& InTransForm);

	void SpawnActorToWorld(TSubclassOf<AActor> Target, AActor*& Spawned, const FTransform& InTransForm);

	template<typename T>
	T* Spawn(TSubclassOf<T> InType, const FTransform& InTransForm)
	{
		return Cast<T>(Spawn(TSubclassOf<AActor>(InType), InTransForm));
	}

	void ReturnPool(AActor* InActor);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void WarmUpAll();
	void WarmUp(TSubclassOf<AActor> InClass);

	UFUNCTION(BlueprintCallable)
	void ClearAllPools();
	void ClearPool(TSubclassOf<AActor> InClass);
	
	void CreatePool(const UObjectPoolDataAsset* InADaata);

	UFUNCTION(BlueprintCallable)
	bool RegisterPoolDataAsset(const UObjectPoolDataAsset* InDataAsset, bool bWarmUp = false);
	UFUNCTION(BlueprintCallable)
	bool UnregisterPoolDataAsset(const UObjectPoolDataAsset* InDataAsset);

protected:


	UPROPERTY(Transient)
	TMap<TSubclassOf<AActor>, FActorPool> PoolMap;
	//TMap<TSubclassOf<AActor>, FActorPool> PoolMap;

	//UPROPERTY(Transient)
	//TSubclassOf<AActor> DamagePopupClass = nullptr;

};
