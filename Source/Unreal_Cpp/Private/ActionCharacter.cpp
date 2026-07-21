// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	WalkSpeed = 300.0f;
	SprintSpeed = 600.0f;
}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Test, ETriggerEvent::Started, this, &AActionCharacter::OnTestAction);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveAction);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AActionCharacter::OnSprintStartAction);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AActionCharacter::OnSprintExitAction);
	}
}

void AActionCharacter::OnTestAction(const FInputActionValue& Value)
{
	
}

void AActionCharacter::OnMoveAction(const FInputActionValue& Value)
{
	
	FVector2D Input2D = Value.Get<FVector2D>();
	FRotator CamRot = GetController()->GetControlRotation();
	FRotator PlayerRot = FRotator(0, CamRot.Yaw, 0);
	//makevectorfrom
	FVector LocalForward = UKismetMathLibrary::GetForwardVector(PlayerRot);
	FVector LocalRight = UKismetMathLibrary::GetRightVector(FRotator(0, CamRot.Yaw, CamRot.Roll));
	AddMovementInput(LocalRight, Input2D.Y);
	AddMovementInput(LocalForward, Input2D.X);
}

void AActionCharacter::OnSprintStartAction(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AActionCharacter::OnSprintExitAction(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


