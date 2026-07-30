


#include "Item/PickupBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/StatHolder.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


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

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	NiagaraComponent->SetupAttachment(RootComponent);
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
	InitPos = GetActorLocation();
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Elapsed += DeltaTime;

	if (bFollow)
	{
		//MoveToPlayerWithTick();
	}
	else
	{
		MoveupDownAndSpinWithTick();
	}
}

void APickupBase::OnPickUp(AActor* InActor)
{

}

bool APickupBase::IsCurveReady() const
{
	return UpDownCurve != nullptr;;
}

void APickupBase::MoveupDownAndSpinWithTick()
{
	//if (Elapsed > 3)
	//	Elapsed -= 3.0f;
	//float Dz = UpDownCurve->GetFloatValue(Elapsed) * MoveWidth;
	//SetActorLocation(InitPos + FVector(0, 0, Dz));

	float Progress = FMath::Fmod(Elapsed / UpDownDuration, 1.0f);

	FVector NewMeshLocation = OffsetInitBP;
	NewMeshLocation.Z += UpDownCurve->GetFloatValue(Progress) * MoveWidth;

	Mesh->SetRelativeLocation(NewMeshLocation);

	float NewAngle = SpinCurve->GetFloatValue(Progress) * 360.0f;
	Mesh->SetRelativeRotation(FRotator(0.0f, NewAngle, 0.0f));
}



