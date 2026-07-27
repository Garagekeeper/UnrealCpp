// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class UNREAL_CPP_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Move(float InVal);

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MainMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector InitPos;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MoveWidth;

	float Elapsed;
};
