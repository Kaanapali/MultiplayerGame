// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(MyLogCategory, Log, All);

UCLASS()
class MULTIPLAYERGAME_API AMyWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent * MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FireEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TraceEffect;

public:	
	// Sets default values for this actor's properties
	AMyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();
	
	UPROPERTY(ReplicatedUsing = OnReplicateHitPoint)
	FVector_NetQuantize HitPoint;

	UFUNCTION()
	void OnReplicateHitPoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();

protected:
	void PlayFireEffects(FVector endPoint);
	void PlayImpactEffects(FVector impactPoint);
};
