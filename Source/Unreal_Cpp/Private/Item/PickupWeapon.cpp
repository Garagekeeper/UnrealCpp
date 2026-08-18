 


#include "Item/PickupWeapon.h"
#include "Weapon/WeaponActor.h"
#include "Unreal_Cpp/Unreal_Cpp.h"
#include "Interface/WeaponUserInterface.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Framework/SubSystem/ObjectPoolSubsystem.h"


APickupWeapon::APickupWeapon()
{
	PickSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionTest"));
	PickSphereCollision->InitSphereRadius(10.0f);
	PickSphereCollision->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void APickupWeapon::OnPickUp(AActor* InTarget)
{
	Super::OnPickUp(InTarget);

	//FActorSpawnParameters SpawnParam;
	//SpawnParam.Owner = InTarget;
	//SpawnParam.Instigator = InTarget->GetInstigator();

	//AWeaponActor* Weapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponData, FTransform::Identity, SpawnParam);
	//Weapon->OnEquippedToTarget(InTarget, ECC_Enemy);

	//IWeaponUserInterface::Execute_

	Target = InTarget;
	InitPos = Mesh->GetComponentLocation();
	DetectPickUp();
}

void APickupWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (WeaponData.IsValid())
	{
		// 여기서는 바로 로딩되는게 좋음
		// 내부에서 get에서 있으면 바로 리턴 없으면 로드
		if (USkeletalMesh* SkeletalData = WeaponData->Mesh.LoadSynchronous())
		{
			Mesh->SetSkeletalMesh(SkeletalData);
			Mesh->SetRelativeLocation(OffsetInitBP + WeaponData->SpwanLocationOffset);
		}
	}
}

void APickupWeapon::Tick(float DeltaTime)
{
	// 내 풀이
	Super::Tick(DeltaTime);
	Elapsed += DeltaTime;

	if (bFollow)
	{
		//MoveToPlayerWithTick();
	}
}

void APickupWeapon::BeginPlay()
{
	Super::BeginPlay();
	PickSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupWeapon::OnHitAreaBeginOverlap);

}

void APickupWeapon::Init(const UItemDataAsset* asset)
{
	Super::Init(asset);
	if (!asset) return;
	if (const UWeaponDataAsset* Casted = Cast<UWeaponDataAsset>(asset))
	{
		WeaponData = Casted;
		if (USkeletalMesh* SkeletalData = WeaponData->Mesh.LoadSynchronous())
		{
			Mesh->SetSkeletalMesh(SkeletalData);
			Mesh->SetWorldLocation(FVector::ZeroVector);
			Mesh->SetRelativeLocation(OffsetInitBP + WeaponData->SpwanLocationOffset);
			ElapsedForTimer = 0;
			NiagaraComponent->Activate();

		}
	}
}

void APickupWeapon::DetectPickUp()
{
	//TODO pickup 먹을 수 있는 인터페이스 만드릭
	if (!Target.Get()->Implements<UWeaponUserInterface>()) return;
	Super::DetectPickUp();
}

UMeshComponent* APickupWeapon::GetMesh() const
{
	return Mesh;
}

void APickupWeapon::MoveToPlayerWithTick()
{
	FVector NewVec = FMath::Lerp(InitPos, Target.Get()->GetActorLocation(), PosCurve->GetFloatValue(Elapsed));
	float ScaleVal = ScaleCurve->GetFloatValue(Elapsed);
	SetActorScale3D(FVector(ScaleVal));
	NewVec = FVector(NewVec.X, NewVec.Y, NewVec.Z + HeightCurve->GetFloatValue(Elapsed));
	SetActorLocation(NewVec);
}

//bool APickupWeapon::IsAssetReady() const
//{
//	return UpDownCurve != nullptr && PosCurve != nullptr && HeightCurve != nullptr && ScaleCurve != nullptr;
//}

//void APickupWeapon::MoveToPlayerWithTimer()
//{
//	if (!Target.IsValid())
//	{
//		MoveToplayerWithTimerDone();
//		return;
//	}
//
//	ElapsedForTimer += TimerInterval;
//
//	PickUpEffectDuration = PickUpEffectDuration <= 0 ? 0.0001f : PickUpEffectDuration;
//	float Progress = ElapsedForTimer / PickUpEffectDuration;
//	if (Progress >= 1.0f)
//	{
//		MoveToplayerWithTimerDone();
//	}
//
//	FVector NewVec = FMath::Lerp(GetActorLocation(), Target.Get()->GetActorLocation(), PosCurve->GetFloatValue(Progress));
//	float ScaleVal = ScaleCurve->GetFloatValue(Progress);
//	SetActorScale3D(FVector(ScaleVal));
//	NewVec = FVector(NewVec.X, NewVec.Y, NewVec.Z + HeightCurve->GetFloatValue(Progress));
//	Mesh->SetWorldLocation(NewVec);
//}
//
void APickupWeapon::MoveToplayerWithTimerDone()
{
	Super::MoveToplayerWithTimerDone();
	if (Target.IsValid() )
	{
		IWeaponUserInterface::Execute_EquipWeapon(Target.Get(), WeaponData.Get());
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			UObjectPoolSubsystem* SubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>();
			SubSystem->ReturnPool(this);
		}

		//Destroy();
	}
}


void APickupWeapon::OnHitAreaBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult)
{
	if (Target.IsValid() && bFollow)
	{
		if (InOtherActor == Target.Get())
		{
			IWeaponUserInterface::Execute_EquipWeapon(InOtherActor, WeaponData.Get());

			if (UGameInstance* GameInstance = GetGameInstance())
			{
				UObjectPoolSubsystem* SubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>();
				SubSystem->ReturnPool(this);
			}
			//Destroy();
		}

	}
}