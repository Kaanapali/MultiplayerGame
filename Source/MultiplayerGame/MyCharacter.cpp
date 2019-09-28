// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyWeapon.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/World.h"


void AMyCharacter::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AMyCharacter, IsChasing);
}

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>
		("SpringArm Component");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>
											("Camera Component");
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	DisableMovement = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority && WeaponClass) {
		FActorSpawnParameters parameters;
		parameters.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponClass,
			FVector::ZeroVector, FRotator::ZeroRotator,
			parameters);

		if (CurrentWeapon) {
			CurrentWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				"WeaponSocket");
			CurrentWeapon->SetOwner(this);
		}
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this,
								&AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this,
								&AMyCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this,
								&AMyCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this,
								&AMyCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this,
								&AMyCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this,
								&AMyCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,
								&AMyCharacter::Jump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this,
								&AMyCharacter::Fire);

	PlayerInputComponent->BindAction("Jog", IE_Pressed, this,
		&AMyCharacter::BeginJog);
	PlayerInputComponent->BindAction("Jog", IE_Released, this,
		&AMyCharacter::EndJog);
}

void AMyCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void AMyCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

void AMyCharacter::BeginCrouch()
{
	Crouch();

	CrouchPressed = true;
}

void AMyCharacter::EndCrouch()
{
	UnCrouch();

	CrouchPressed = false;
}

void AMyCharacter::BeginJog()
{
	JogPressed = true;
}

void AMyCharacter::EndJog()
{
	JogPressed = false;
}

void AMyCharacter::Fire()
{
	if (CurrentWeapon) {
		CurrentWeapon->Fire();
	}
}

FVector AMyCharacter::GetPawnViewLocation() const
{
	if (CameraComponent) {
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}