


#include "Item/PickupBase.h"
#include "Components/MeshComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/StatHolder.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Framework/SubSystem/ObjectPoolSubsystem.h"
#include "Unreal_Cpp/Unreal_Cpp.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	DetectSphereCollision->InitSphereRadius(100.0f);
	DetectSphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectSphereCollision->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	SetRootComponent(DetectSphereCollision);



	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void APickupBase::ReturnPoolObject()
{

}

void APickupBase::Init(UItemDataAsset* asset)
{
	SetActorScale3D(FVector::OneVector);

	if (UMeshComponent* PickupMesh = GetMesh())
	{
		PickupMesh->SetRelativeLocation(FVector::ZeroVector);
	}
	bFollow = false;
	Elapsed = 0;
	DetectSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APickupBase::Onspawn_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void APickupBase::OnReturn_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetActorLocation(FVector(0, 0, -10000.0f));
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


	if (UMeshComponent* PickupMesh = GetMesh())
	{
		UpDownDuration = UpDownDuration <= 0 ? 0.0001f : UpDownDuration;
		float Progress = FMath::Fmod(Elapsed / UpDownDuration, 1.0f);

		FVector NewMeshLocation = OffsetInitBP;
		NewMeshLocation.Z += UpDownCurve->GetFloatValue(Progress) * MoveWidth;
		PickupMesh->SetRelativeLocation(NewMeshLocation);

		float NewAngle = SpinCurve->GetFloatValue(Progress) * 360.0f;
		PickupMesh->SetRelativeRotation(FRotator(0.0f, NewAngle, 0.0f));
	}


}
UMeshComponent* APickupBase::GetMesh() const
{
	return nullptr;
}



