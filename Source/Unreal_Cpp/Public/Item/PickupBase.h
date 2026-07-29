

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class UNREAL_CPP_API APickupBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APickupBase();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnPickUp(AActor* InActor);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> DetectSphereCollision = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	
	
};
