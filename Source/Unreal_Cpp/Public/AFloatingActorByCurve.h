// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloatingActor.h"
#include "AFloatingActorByCurve.generated.h"


UCLASS()
class UNREAL_CPP_API AAFloatingActorByCurve : public AFloatingActor
{
	GENERATED_BODY()
	
public:
	AAFloatingActorByCurve();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Move(float InVal) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* MyCurve;

};
