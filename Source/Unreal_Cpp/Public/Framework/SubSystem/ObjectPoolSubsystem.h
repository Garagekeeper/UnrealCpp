

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

/**
 * 
 */
USTRUCT()
struct FActorPool
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> ReadyActors;

	// 실제 사용 중인 액터들
	UPROPERTY(Transient)
	TSet<TObjectPtr<AActor>> ActiveActors;
};

UCLASS()
class UNREAL_CPP_API UObjectPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//template<typename T>
	AActor* Spawn(AActor* InType, const FTransform& InTransForm);
	void ReturnPool(AActor* InActor);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void CreatePool(TSoftClassPtr<AActor> InActor);

protected:
	// 사용 대기중인 액터들
	// 사실상 여기가 풀임
	//UPROPERTY(Transient)
	//TArray<TObjectPtr<AActor>> ReadyActors;

	//// 실제 사용 중인 액터들
	//UPROPERTY(Transient)
	//TSet<TObjectPtr<AActor>> ActiveActors;

	UPROPERTY(Transient)
	TMap<UClass*, FActorPool> PoolMap;

	UPROPERTY(Transient)
	TSubclassOf<AActor> DamagePopupClass = nullptr;

	UPROPERTY(Transient)
	TMap<UClass*, TSubclassOf<AActor>> PoolableClass;

protected:
	int InitialPoolSize = 10;
	int MaxPoolSize = 30;

	
};
