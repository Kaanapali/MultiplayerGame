// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyHealthComponent.h"
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

	//////////////////////////////////////////////////////////////////////////
	// Movement

	/** [server + local] change running state */
	void SetRunning(bool bNewRunning, bool bToggle);

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
	void OnStartRunning();
	void OnStopRunning();
	void OnStartJump();
	void OnStopJump();
	
	void Fire();

	UFUNCTION()
	void OnHealthChanged(UMyHealthComponent* healthComp, 
					float health, float damage, 
					const class UDamageType* damageType, 
					class AController* instigatedBy, AActor* damageCauser);
	
	/** update running state */
	UFUNCTION(reliable, server, WithValidation)
	void ServerSetRunning(bool bNewRunning, bool bToggle);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMyHealthComponent * HealthComponent;

	UPROPERTY(Replicated)
	class AMyWeapon * CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AMyWeapon> WeaponClass;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bDisableMovement;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Movement")
	bool bCrouchPressed;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Movement")
	bool bPronePressed;

	/** current running state */
	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
	bool bJogPressed;
};
