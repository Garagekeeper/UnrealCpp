// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "../Interface/StaminaInterface.h"


// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	SphereCollision->InitSphereRadius(100.0f);
	SetRootComponent(SphereCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//1. 인터페이스로 캐스트
	//IStaminaInterface* Target =  Cast<IStaminaInterface>(OtherActor);

	//2. 권장하는 방법(Bp에서 상속 받은 것도 체크 가능)
	//bool bImplements = OtherActor->Implements<UStaminaInterface>();

	if (OtherActor && OtherActor->Implements<UStaminaInterface>())
	{
		if (Stamina > 0)
		{
			IStaminaInterface::Execute_RecoveryStamina(OtherActor, Stamina);
		}
		else if (Stamina < 0)
		{
			IStaminaInterface::Execute_ConsumeStamina(OtherActor, -Stamina);
		}
	}

}

