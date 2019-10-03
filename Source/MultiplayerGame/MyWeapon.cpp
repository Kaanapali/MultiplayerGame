// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "MultiplayerGame.h"
#include "MyCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#define COLLISION_WEAPON ECC_GameTraceChannel1

DEFINE_LOG_CATEGORY(MyLogCategory)

void AMyWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&
											OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AMyWeapon, HitPoint, COND_SkipOwner);
}

void AMyWeapon::OnReplicateHitPoint()
{
	PlayFireEffects(HitPoint);
	PlayImpactEffects(HitPoint);
}

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

void AMyWeapon::ServerFire_Implementation()
{
	Fire();
}

bool AMyWeapon::ServerFire_Validate()
{
	return true;
}

void AMyWeapon::Fire()
{
	UE_LOG(MyLogCategory, Log, TEXT("FIRE2"));
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}

	AActor * owner = GetOwner();
	if (owner) {
		FVector eyeLoc;
		FRotator eyeRot;
		owner->GetActorEyesViewPoint(eyeLoc, eyeRot);
		
		FVector endPoint = eyeLoc + (eyeRot.Vector() * 10000);

		FCollisionQueryParams cparams;
		cparams.AddIgnoredActor(owner);
		cparams.AddIgnoredActor(this);
		cparams.bTraceComplex = true;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLoc, endPoint,
												COLLISION_WEAPON, cparams)) {
			AActor * hitActor = hit.GetActor();
			UGameplayStatics::ApplyPointDamage(hitActor, 10.0f,
				eyeRot.Vector(), hit,
				owner->GetInstigatorController(),
				this, DamageType);
			PlayImpactEffects(hit.ImpactPoint);
			endPoint = hit.ImpactPoint;
			/*
			AMyCharacter* ownerChar = Cast<AMyCharacter>(owner);
			ownerChar->SpawnSimpleStuff(endPoint);
			*/
			UE_LOG(MyLogCategory, Log, TEXT("-%s-: endpoint : %s"),
				*GetNameSafe(hitActor), *endPoint.ToString());
		}

		PlayFireEffects(endPoint);
		if (Role == ROLE_Authority) {
			HitPoint = endPoint;
		}

		FVector startPoint = MeshComponent->GetSocketLocation("Muzzle");
		DrawDebugLine(GetWorld(), startPoint, endPoint, FColor::Red,
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

void AMyWeapon::PlayImpactEffects(FVector impactPoint)
{
	if (ImpactEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, impactPoint);
	}
}