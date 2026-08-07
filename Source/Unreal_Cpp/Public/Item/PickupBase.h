

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Poolable.h"
#include "PickupBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UItemDataAsset;

UCLASS()
class UNREAL_CPP_API APickupBase : public AActor, public IPoolable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APickupBase();
	
	UFUNCTION(BlueprintCallable)
	virtual void ReturnPoolObject() override;
	virtual void Init(UItemDataAsset* asset);

	void Onspawn_Implementation();
	void OnReturn_Implementation();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnPickUp(AActor* InActor);
	virtual void MoveupDownAndSpinWithTick();
	virtual UMeshComponent* GetMesh() const ;

private:
	bool IsCurveReady() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Transform")
	TObjectPtr<UCurveFloat> UpDownCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Transform")
	TObjectPtr<UCurveFloat> SpinCurve;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> DetectSphereCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Transform")
	float MoveWidth = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0001"))
	float UpDownDuration = 5;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Data")
	FVector OffsetInitBP = FVector(0.0f, 0, 20);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Data")
	TObjectPtr<UItemDataAsset> DataAsset = nullptr;

	FVector InitPos;
	bool bFollow = false;
	float Elapsed = 0;



	
};
