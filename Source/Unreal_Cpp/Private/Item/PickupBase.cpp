


#include "Item/PickupBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/StatHolder.h"


// Sets default values
APickupBase::APickupBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	DetectSphereCollision->InitSphereRadius(100.0f);
	SetRootComponent(DetectSphereCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

void APickupBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	OnPickUp(OtherActor);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBase::OnPickUp(AActor* InActor)
{

}


