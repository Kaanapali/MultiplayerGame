// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class MULTIPLAYERGAME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void BeginCrouch();
	void EndCrouch();
	void BeginJog();
	void EndJog();

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
};
