


#include "Enemy/DamagepopupActor.h"
#include "Components/WidgetComponent.h"
#include "Widget/DamagePopupWidget.h"
#include "Framework/SubSystem/ObjectPoolSubsystem.h"

// Sets default values
ADamagepopupActor::ADamagepopupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DamagePopupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FmamagePopupWidgetComp"));
	SetRootComponent(DamagePopupWidgetComponent);

	DamagePopupWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DamagePopupWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADamagepopupActor::OnPopupStart(float InDamage)
{
	DamagePopupWidget->SetDamage(InDamage);
	DamagePopupWidget->PlayPopupAnimation();
}

void ADamagepopupActor::ReturnPoolObject()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UObjectPoolSubsystem* SubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>();
		SubSystem->ReturnPool(this);
	}
}

void ADamagepopupActor::Onspawn_Implementation()
{
	SetActorHiddenInGame(false);
}

void ADamagepopupActor::OnReturn_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorLocation(FVector(0, 0, -10000.0f));
}

// Called when the game starts or when spawned
void ADamagepopupActor::BeginPlay()
{
	Super::BeginPlay();
	if (!DamagePopupWidget)
	{
		DamagePopupWidget = Cast<UDamagePopupWidget>(DamagePopupWidgetComponent->GetWidget());
		DamagePopupWidget->OnPopupAnimationFinished.BindUObject(this, &ADamagepopupActor::ReturnPoolObject);
	}

}

// Called every frame
void ADamagepopupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

