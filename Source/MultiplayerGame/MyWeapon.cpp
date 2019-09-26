// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>
											("Mesh Component");
	RootComponent = MeshComponent;
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

		FVector endPoint = eyeLoc + (eyeRot.Vector() * 1000);

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
		}

		DrawDebugLine(GetWorld(), eyeLoc, endPoint, FColor::Red,
			false, 1.0f, 0, 1);
	}
}