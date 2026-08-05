
#include "Weapon/WeaponActor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Unreal_Cpp/Unreal_Cpp.h"
#include "Interface/WeaponUserInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Data/WeaponDataAsset.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/SkeletalMesh.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(Mesh);

	//Mesh->SetCollisionProfileName(TEXT("NoCollision"));						// 1.프로파일을 통해서 한번에 세팅
	Mesh->SetGenerateOverlapEvents(false);										// 5.오버렙 이벤트 발생 여부
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);				// 2.열거형을 통해서 세팅
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);			// 3.오브젝트 타입 바꾸기
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);	// 4.채널별 반응 조절

	HitArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitArea"));
	HitArea->SetupAttachment(Mesh);

	HitArea->SetCapsuleHalfHeight(70.0f, false);								// 뒤의 bool은 크기 변경시 오버렙 바로 갱신할거냐
	HitArea->SetCapsuleRadius(30.0f, false);
	HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//HitArea->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HitArea->SetCollisionObjectType(ECC_Weapon);
	HitArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitArea->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);

	TrailVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailVFX"));
	TrailVFX->SetupAttachment(RootComponent);
	TrailVFX->bAutoActivate = false;
	TrailVFX->Deactivate();
}

void AWeaponActor::AttackEnable(bool bEnable)
{
	if (bEnable)
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		TrailVFX->Activate(true);
	}
	else
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TrailVFX->Deactivate();
		if (UsageCount > 0)
		{
			UsageCount--;
			if (UsageCount == 0)
			{
				DropWeapon();
				IWeaponUserInterface::Execute_EquipWeapon(GetOwner(), nullptr);
			}

		}
	}
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	HitArea->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnHitAreaBeginOverlap);
	//UE_LOG(LogTemp, Log, TEXT("웨폰액터 시작"));
}

void AWeaponActor::OnEquipped(AActor* InOwner, ECollisionChannel TargetChanel)
{
	if (!WeaponData) return;

	SetOwner(InOwner);
	TrailVFX->Deactivate();
	OwnerCharacter = Cast<ACharacter>(InOwner);
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true
	);

	if (OwnerCharacter.IsValid())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), AttachRules, AttachSocketName);
		HitArea->SetRelativeLocation(WeaponData->LocationOffset);
		
		HitArea->IgnoreActorWhenMoving(OwnerCharacter.Get(), true);// 자기 자신이랑은 충돌 안하게 (만약을 대비)

		IWeaponUserInterface* WeaponUser = Cast<IWeaponUserInterface>(OwnerCharacter);
		if (WeaponUser)
		{
			WeaponUser->GetWeaponAttackStateChangedDelegate().BindUFunction(this, FName("AttackEnable"));
			//WeaponUser->SetWeapon(this);
		}

		if (TargetChanel == ECC_Enemy)
		{
			HitArea->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
			HitArea->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
		}
		else if (TargetChanel == ECC_Player)
		{
			HitArea->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
			HitArea->SetCollisionResponseToChannel(ECC_Enemy, ECR_Ignore);
			HitArea->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	
}

void AWeaponActor::OnEquippedToTarget(AActor* InOwner, ECollisionChannel TargetChanel)
{
	OnEquipped(InOwner, TargetChanel);
}

void AWeaponActor::InitalizeWeapon(UWeaponDataAsset* InData)
{
	if (!InData) nullptr;

	WeaponData = InData;
	Mesh->SetSkeletalMesh(WeaponData->Mesh.Get());

	HitArea->SetCapsuleHalfHeight(WeaponData->hitAreaHalfheight, false);								// 뒤의 bool은 크기 변경시 오버렙 바로 갱신할거냐
	HitArea->SetCapsuleRadius(WeaponData->hitAreaRad, false);

	HitArea->SetRelativeLocation(WeaponData->LocationOffset);

	AttachSocketName = WeaponData->AttachSocketName;
	AttackDamage = WeaponData->AttackDamage;
	UsageCount = WeaponData->UsageCount;

	TrailVFX->SetAsset(WeaponData->WeaponTrailVFX.Get());
	//UE_LOG(LogTemp, Log, TEXT("%p"), WeaponData->WeaponTrailVFX.Get());
}

void AWeaponActor::DropWeapon()
{
	TrailVFX->Deactivate();
	FDetachmentTransformRules DetachRues(EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachRues);

	HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	Mesh->SetSimulatePhysics(true);

	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(
		PhysicsDelayTimerHandle,
		FTimerDelegate::CreateWeakLambda(
			this,
			[this]()
			{
				if (Mesh)
				{
					Mesh->SetCollisionResponseToChannel(ECC_Player, ECR_Block);
				}
			}
		),
		PhysicsDelay,
		false
	);


	// 뒤로 던지기
	FVector BackDir = OwnerCharacter.IsValid() ? -OwnerCharacter->GetActorForwardVector() : -FVector::BackwardVector;
	FVector ThrowDir = BackDir * 500.0f + FVector::UpVector * 300.0f;
	FVector AngularImpulse = FVector(
		FMath::RandRange(-200, 200)
	) + GetActorForwardVector()*1000;
	Mesh->AddImpulse(ThrowDir, NAME_None, true);
	Mesh->AddAngularImpulseInDegrees(AngularImpulse, NAME_None, true);

	// 삭제 시간 걸어주기
	SetLifeSpan(DropLifeSpan);

	// 오너 캐릭터 밀어주기
	OwnerCharacter = nullptr;
}

void AWeaponActor::RestUseCnt()
{
	UsageCount = WeaponData->UsageCount;
}

FVector AWeaponActor::GetWeaponImpactLocation() const
{
	return FMath::Lerp(Mesh->GetSocketLocation(TEXT("Tip")), Mesh->GetSocketLocation(TEXT("Base")), 0.5f);
}

void AWeaponActor::SetActivator(bool bActive)
{
	// 1. Toggle Actor Ticking
	SetActorTickEnabled(bActive);

	// 2. Toggle Actor Visibility (Renders/Hides all attached primitives)
	SetActorHiddenInGame(!bActive);

	// 3. Toggle Actor Collision
	SetActorEnableCollision(bActive);

	

}


void AWeaponActor::OnHitAreaBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult)
{
	if (!OwnerCharacter.IsValid() || !InOtherActor) return;
	if (!WeaponData) return;

	//UE_LOG(LogTemp, Log, TEXT("오버랩 된 대상 : %s"), *InOtherActor->GetName());
	float Remain = UGameplayStatics::ApplyDamage(InOtherActor, AttackDamage, OwnerCharacter->GetController(), this, nullptr);
}

