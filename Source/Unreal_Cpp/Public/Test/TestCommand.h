

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestCommand.generated.h"

UCLASS()
class UNREAL_CPP_API ATestCommand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestCommand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UFUNCTION(BlueprintCallable)
	void Move();

	UFUNCTION(BlueprintCallable)
	void Use();

	UFUNCTION(BlueprintCallable)
	void Drop();

	
};
