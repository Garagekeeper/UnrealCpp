// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ProgressBar.h"

// Sets default values
AActionCharacter::AActionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	CameraSpringArmComponent->bUsePawnControlRotation = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("/Game/Blueprints/Test/Test02/WBP_Stamina.WBP_Stamina_C"));
	if (WidgetFinder.Succeeded())
	{
		StaminaWidget = WidgetFinder.Class;
	}
}

float AActionCharacter::GetCurrentStamina_Implementation() const
{
	return CurrentStamina;
}

bool AActionCharacter::ConsumeStamina_Implementation(float InAmount)
{
	bool bResult = false;
	if (CurrentStamina >= InAmount)
	{
		CurrentStamina -= InAmount;
		bResult = true;
	}

	CurrentStamina = FMath::Clamp(CurrentStamina, 0, MaxStamina);
	NonuseElapsed = 0;
	//UE_LOG(LogTemp, Log, TEXT("현재 Stamina : %.1f"), CurrentStamina);
	return bResult;
}

void AActionCharacter::RecoveryStamina_Implementation(float InAmount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + InAmount, 0, MaxStamina);
	//UE_LOG(LogTemp, Log, TEXT("현재 Stamina : %.1f"), CurrentStamina);
}

float AActionCharacter::GetMaxStamina_Implementation() const
{
	return FMath::IsNearlyZero(MaxStamina) ? 100.0f : MaxStamina;
}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}

	CurrentStamina = MaxStamina;


	if (StaminaWidget)
	{
		CreatedWidget = CreateWidget<UUserWidget>(GetWorld(), StaminaWidget);
		if (CreatedWidget)
		{
			CreatedWidget->AddToViewport();
		}
	}
	StaminaBar = Cast<UProgressBar>(CreatedWidget->GetWidgetFromName(TEXT("StaminaBar")));
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRun)
	{
		if(GetMovementComponent()->Velocity.Size() >= WalkSpeed)
		IStaminaInterface::Execute_ConsumeStamina(this, DeltaTime * 10);
	}
	
	NonuseElapsed += DeltaTime;

	if (NonuseElapsed >= 3)
	{
		if (!FMath::IsNearlyEqual(CurrentStamina,MaxStamina))
			IStaminaInterface::Execute_RecoveryStamina(this, DeltaTime * 20);
	}

	auto Val = FMath::FInterpTo(StaminaBar->GetPercent(), CurrentStamina/MaxStamina, DeltaTime, 5);
	StaminaBar->SetPercent(Val);
}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Test, ETriggerEvent::Started, this, &AActionCharacter::OnTestAction);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveAction);
		//EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AActionCharacter::OnSprintStartAction);
		//EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AActionCharacter::OnSprintExitAction);
		EnhancedInputComponent->BindActionValueLambda(IA_Sprint, ETriggerEvent::Started,
			[this](const FInputActionValue& _) {
				OnSprintStart();
			});
		EnhancedInputComponent->BindActionValueLambda(IA_Sprint, ETriggerEvent::Completed,
			[this](const FInputActionValue& _) {
				OnSprintEnd();
			});

		EnhancedInputComponent->BindAction(IA_Roll, ETriggerEvent::Started, this, &AActionCharacter::OnRollAction);
	}
}

void AActionCharacter::OnTestAction(const FInputActionValue& Value)
{
	
}

void AActionCharacter::OnMoveAction(const FInputActionValue& Value)
{
	
	FVector2D Input2D = Value.Get<FVector2D>();

	/*FRotator CamRot = GetController()->GetControlRotation();
	FRotator PlayerRot = FRotator(0, CamRot.Yaw, 0);
	FVector LocalForward = UKismetMathLibrary::GetForwardVector(PlayerRot);
	FVector LocalRight = UKismetMathLibrary::GetRightVector(FRotator(0, CamRot.Yaw, CamRot.Roll));
	AddMovementInput(LocalRight, Input2D.X);
	AddMovementInput(LocalForward, Input2D.Y);*/

	FVector WorldDirection = FVector(Input2D.Y, Input2D.X, 0).GetSafeNormal();
	
	double CotrollerYawRadVal = FMath::DegreesToRadians(GetControlRotation().Yaw);
	FQuat ControlYawRotation(FVector::UpVector, CotrollerYawRadVal);
	WorldDirection = ControlYawRotation.RotateVector(WorldDirection);

	AddMovementInput(WorldDirection);
	
}

void AActionCharacter::OnSprintStartAction(const FInputActionValue& Value)
{
	if (FMath::IsNearlyZero(CurrentStamina)) return;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AActionCharacter::OnSprintExitAction(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AActionCharacter::OnSprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	bRun = true;
}

void AActionCharacter::OnSprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bRun = false;
}

// play montage example
void AActionCharacter::OnRollAction(const FInputActionValue& Value)
{
	if (!RollMontage.IsValid()) return;

	// 스테미너 모자라면 못구름
	if (IStaminaInterface::Execute_GetCurrentStamina(this) < RollStaminaUsage)  return;

	if (!AnimInstance)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}

	// 다른 몽타주가 재생중이면 스킵
	if (!AnimInstance->IsAnyMontagePlaying())
	{
		// 구르기 직전에 구를 방향으로 돌리기
		// 이것만 있으면 입력이 없는 경우 무조건 정면으로 회전해버림
		// 입력이 없는경우 last input이 0,0 이니까 회전이 없어서 무조건 X축으로 구른다
		// SetActorRotation(GetLastMovementInputVector().Rotation());
		// UE_LOG(LogTemp, Log, TEXT("%f, %f"), GetLastMovementInputVector().X, GetLastMovementInputVector().Y);
		
		// 이게 있어야 실제 유효한 회전을 넣음
		// 입력이 없어도 현재 바라보는 방향으로 구른다.
		if (!GetLastMovementInputVector().IsNearlyZero())
		{
			SetActorRotation(GetLastMovementInputVector().Rotation());
		}

		PlayAnimMontage(RollMontage.Get());
		IStaminaInterface::Execute_ConsumeStamina(this, RollStaminaUsage);
	}
}


