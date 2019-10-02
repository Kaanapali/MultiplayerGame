// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class MULTIPLAYERGAME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnSimpleStuff(FVector loc);

	// for testing purpose: spawn a stuff right away.
	void SpawnSimpleStuff(FVector loc);

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handles input for moving forward and backward.
	UFUNCTION()
	void MoveForward(float value);

	// Handles input for moving right and left.
	UFUNCTION()
	void MoveRight(float value);

	void DoCrouch();
	void DoProne();
	void BeginJog();
	void EndJog();
	void BeginJump();
	void EndJump();
	
	void Fire();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent * CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent * SpringArmComponent;

	UPROPERTY(Replicated)
	class AMyWeapon * CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AMyWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	uint8 bDisableMovement : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	uint8 bJogPressed : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	uint8 bCrouchPressed : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	uint8 bJumpPressed : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	uint8 bPronePressed : 1;
};
