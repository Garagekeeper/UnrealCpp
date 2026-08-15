


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

void APickupBase::Init(const UItemDataAsset* asset)
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
	SetActorEnableCollision(false);
	SetActorTickEnabled(true);
	DetectSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().ClearTimer(PickupEffectTimerHandle);
	Target = nullptr;
	InitPos = GetActorLocation();
	bFollow = false;
	Elapsed = 0;
	ElapsedForTimer = 0;

	GetWorldTimerManager().SetTimer(
		PickupCollisionTimerHandle,
		FTimerDelegate::CreateLambda(
			[this]() 
			{
				this->SetActorEnableCollision(true);
				DetectSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}),
		1,
		false
	);
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
	Target = InActor;
	DetectPickUp();
}

bool APickupBase::IsCurveReady() const
{
	return UpDownCurve != nullptr && PosCurve != nullptr && HeightCurve != nullptr 
		&& ScaleCurve != nullptr && SpinCurve != nullptr;
}

void APickupBase::DetectPickUp()
{

	// 내 풀이
	bFollow = true;
	NiagaraComponent->Deactivate();
	InitPos = GetActorLocation();
	DetectSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorEnableCollision(false);
	
	// 타이머 기반 풀이

	if (IsCurveReady())
	{
		// 해당 타이머가 작동중dlaus 종료
		if (GetWorldTimerManager().IsTimerActive(PickupEffectTimerHandle)) return;

		DetectSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ElapsedForTimer = 0.0f;
		GetWorldTimerManager().SetTimer(
			PickupEffectTimerHandle,
			this,
			&APickupBase::MoveToPlayerWithTimer,
			TimerInterval,
			true
		);
	}
	else
	{
		MoveToplayerWithTimerDone();
	}

}

void APickupBase::MoveToPlayerWithTimer()
{
	if (!GetMesh()) return;
	if (!Target.IsValid())
	{
		MoveToplayerWithTimerDone();
		return;
	}

	ElapsedForTimer += TimerInterval;

	PickUpEffectDuration = PickUpEffectDuration <= 0 ? 0.0001f : PickUpEffectDuration;
	float Progress = ElapsedForTimer / PickUpEffectDuration;
	if (Progress >= 1.0f)
	{
		MoveToplayerWithTimerDone();
		return;
	}


	FVector NewVec = FMath::Lerp(GetActorLocation(), Target.Get()->GetActorLocation(), PosCurve->GetFloatValue(Progress));
	float ScaleVal = ScaleCurve->GetFloatValue(Progress);
	SetActorScale3D(FVector(ScaleVal));
	NewVec = FVector(NewVec.X, NewVec.Y, NewVec.Z + HeightCurve->GetFloatValue(Progress));
	GetMesh()->SetWorldLocation(NewVec);
}

void APickupBase::MoveToplayerWithTimerDone()
{
	GetWorldTimerManager().ClearTimer(PickupEffectTimerHandle);
}

void APickupBase::MoveupDownAndSpinWithTick()
{

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



