// Fill out your copyright notice in the Description page of Project Settings.


#include "AFloatingActorByCurve.h"

AAFloatingActorByCurve::AAFloatingActorByCurve()
	:Super()
{

}


void AAFloatingActorByCurve::BeginPlay()
{
	Super::BeginPlay();
	InitPos = GetActorLocation();
}

void AAFloatingActorByCurve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAFloatingActorByCurve::Move(float InVal)
{
	if (Elapsed > 3)
		Elapsed -= 3.0f;
	float Dz = MyCurve->GetFloatValue(Elapsed) * MoveWidth;
	SetActorLocation(InitPos + FVector(0, 0, Dz));
}
