
#include "Weapon/WeaponActor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Unreal_Cpp/Unreal_Cpp.h"
#include "Interface/WeaponUserInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Data/WeaponDataAsset.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
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
	HitArea->SetRelativeLocation(FVector(0.0f, 0.0f, 61.0f));
}

void AWeaponActor::AttackEnable(bool bEnable)
{
	if (bEnable)
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	HitArea->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnHitAreaBeginOverlap);
	UE_LOG(LogTemp, Log, TEXT("웨폰액터 시작"));
}

void AWeaponActor::OnEquipped(AActor* InOwner, ECollisionChannel TargetChanel)
{
	SetOwner(InOwner);
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
	WeaponData = InData;
	Mesh->SetStaticMesh(WeaponData->Mesh.Get());

	HitArea->SetCapsuleHalfHeight(WeaponData->hitAreaHalfheight, false);								// 뒤의 bool은 크기 변경시 오버렙 바로 갱신할거냐
	HitArea->SetCapsuleRadius(WeaponData->hitAreaRad, false);

	HitArea->SetRelativeLocation(WeaponData->LocationOffset);

	AttachSocketName = WeaponData->AttachSocketName;
	AttackDamage = WeaponData->AttackDamage;

	// 피봇 조정
}

void AWeaponActor::DropWeapon()
{
	FDetachmentTransformRules DetachRues(EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachRues);

	HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	Mesh->SetSimulatePhysics(true);

	Mesh->AddImpulse(GetActorUpVector() * 500, NAME_None, true);

}


void AWeaponActor::OnHitAreaBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult)
{
	//UE_LOG(LogTemp, Log, TEXT("오버랩 된 대상 : %s"), *InOtherActor->GetName());
	float Remain = UGameplayStatics::ApplyDamage(InOtherActor, AttackDamage, OwnerCharacter->GetController(), this, nullptr);
	UE_LOG(LogTemp, Log, TEXT("대상: %s, 남은 체력: %f"), *InOtherActor->GetName(), Remain);
}

