// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>
											("Mesh Component");
	RootComponent = MeshComponent;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyWeapon::Fire()
{
	AActor * owner = GetOwner();
	if (owner) {
		FVector eyeLoc;
		FRotator eyeRot;
		owner->GetActorEyesViewPoint(eyeLoc, eyeRot);
		
		FVector endPoint = eyeLoc + (eyeRot.Vector() * 100);

		FCollisionQueryParams cparams;
		cparams.AddIgnoredActor(owner);
		cparams.AddIgnoredActor(this);
		cparams.bTraceComplex = true;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLoc, endPoint,
												ECC_Visibility, cparams)) {
			AActor * hitActor = hit.GetActor();
			UGameplayStatics::ApplyPointDamage(hitActor, 10.0f,
				eyeRot.Vector(), hit,
				owner->GetInstigatorController(),
				this, DamageType);

			PlayImpactEffects(hit.ImpactPoint);
			endPoint = hit.ImpactPoint;
		}

		PlayFireEffects(endPoint);

		DrawDebugLine(GetWorld(), eyeLoc, endPoint, FColor::Red,
			false, 1.0f, 0, 1);
	}
}

void AMyWeapon::PlayFireEffects(FVector endPoint)
{
	if (FireEffect) {
		UGameplayStatics::SpawnEmitterAttached(FireEffect, MeshComponent, "Muzzle");
	}

	if (TraceEffect) {
		UParticleSystemComponent* inst = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			TraceEffect, MeshComponent->GetSocketLocation("Muzzle"));
		inst->SetVectorParameter("Target", endPoint);
	}
}

void AMyWeapon::PlayImpactEffects(FVector endPoint)
{
	if (ImpactEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, endPoint);
	}
}