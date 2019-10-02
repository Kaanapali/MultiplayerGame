// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MultiplayerGame.h"
#include "MyWeapon.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

void AMyCharacter::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyCharacter, CurrentWeapon);
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

	bDisableMovement = false;
	bCrouchPressed = false;

	HealthComponent = CreateDefaultSubobject<UMyHealthComponent>
		("Health Component");

	SetReplicates(true);
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
								&AMyCharacter::DoCrouch);
	PlayerInputComponent->BindAction("Prone", IE_Pressed, this,
								&AMyCharacter::DoProne);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,
								&AMyCharacter::BeginJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this,
								&AMyCharacter::EndJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this,
								&AMyCharacter::Fire);

	PlayerInputComponent->BindAction("Jog", IE_Pressed, this,
		&AMyCharacter::BeginJog);
	PlayerInputComponent->BindAction("Jog", IE_Released, this,
		&AMyCharacter::EndJog);
}

void AMyCharacter::MoveForward(float value)
{
	if (!bDisableMovement) {
		//FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		FVector Direction = GetActorForwardVector();
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AMyCharacter::MoveRight(float value)
{
	if (!bDisableMovement)
		AddMovementInput(GetActorRightVector(), value);
}

void AMyCharacter::DoCrouch()
{
	// Crouch();
	/**
	 * because we don't have an animation to go from Prone to Crouch and vice-versa. 
	 * If you had the animations, you would probably want to allow the player to enter a Prone state from Crouch
	 */
	if (bPronePressed)
		return;

	if (!bCrouchPressed) {
		bCrouchPressed = true;
		GetCharacterMovement()->MaxWalkSpeed = 160.0f;
	}
	else {
		bCrouchPressed = false;
		if (bJogPressed)
			GetCharacterMovement()->MaxWalkSpeed = 375.0f;
		else
			GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}	
}

void AMyCharacter::DoProne()
{
	/**
	 * check if we are currently crouched and if so, 
	 * not allowing the character to enter a prone state 
	 * (as mentioned, since we do not have the transition animations).
	 */ 
	if (bCrouchPressed)
		return;

	// TODO: need adjust
	if (!bPronePressed) {
		bPronePressed = true;
		GetCharacterMovement()->MaxWalkSpeed = 60.0f;
	}
	else {
		bPronePressed = false;
		GetCharacterMovement()->MaxWalkSpeed = 160.0f;
	}	
}

void AMyCharacter::BeginJog()
{
	if (bDisableMovement)
		return;

	bJogPressed = true;

	GetCharacterMovement()->MaxWalkSpeed = 375.0f;
}

void AMyCharacter::EndJog()
{
	bJogPressed = false;

	if (bCrouchPressed)
		GetCharacterMovement()->MaxWalkSpeed = 160.0f;
	else
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void AMyCharacter::BeginJump()
{
	if (bDisableMovement)
		return;

	if (!bCrouchPressed || (bCrouchPressed && GetCharacterMovement()->MaxWalkSpeed > 160.0f)) {
		if (GetCharacterMovement()->Velocity.Size() > 0) {
			Jump();
			GetCharacterMovement()->JumpZVelocity = 365;
		}
		else {
			Jump();
			GetCharacterMovement()->JumpZVelocity = 340;
		}
	}

	bJumpPressed = true;
}

void AMyCharacter::EndJump()
{
	bJumpPressed = false;
}

void AMyCharacter::Fire()
{
	if (CurrentWeapon) {
		CurrentWeapon->Fire();
	}
	else {
		UE_LOG(MyLogCategory, Error, TEXT("CurentWeapon is nullptr!"));

	}
}

FVector AMyCharacter::GetPawnViewLocation() const
{
	if (CameraComponent) {
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void AMyCharacter::ServerSpawnSimpleStuff_Implementation(FVector loc)
{
	SpawnSimpleStuff(loc);
}

bool AMyCharacter::ServerSpawnSimpleStuff_Validate(FVector loc)
{
	return true;
}

void AMyCharacter::SpawnSimpleStuff(FVector loc)
{
	if (Role < ROLE_Authority)
	{
		ServerSpawnSimpleStuff(loc);
	}

	UClass* MyItemBlueprintClass = StaticLoadClass(AActor::StaticClass(),
		NULL, TEXT("/Game/SimpleStuff.SimpleStuff_C"),
		NULL, LOAD_None, NULL);
	UE_LOG(MyLogCategory, Log, TEXT("-%d-"), MyItemBlueprintClass);
	FActorSpawnParameters parameters;
	parameters.Owner = this;
	parameters.Instigator = Instigator;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(MyItemBlueprintClass, loc, FRotator::ZeroRotator, parameters);
}
