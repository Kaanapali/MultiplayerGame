// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHealthComponent.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor * myOwner = GetOwner();
	if (myOwner) {
		myOwner->OnTakeAnyDamage.AddDynamic(this, &UMyHealthComponent::OnTakeDamage);
	}
}


// Called every frame
void UMyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UMyHealthComponent::OnTakeDamage(AActor* DamageActor, float Damage, 
	const class UDamageType * DamageType,
	class AController* InstigateBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, 99.99f);
	UE_LOG(LogTemp, Warning, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));
	
	OnHealthChangedEvent.Broadcast(this, Health, Damage, DamageType,
		InstigateBy, DamageCauser);
}