// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"


// define a dynamic, multicase, delegate, 6 params
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, 
					UMyHealthComponent*, HealthComp, float, Health, float, HealthDelta, 
					const class UDamageType*, DamageType, class AController*,
					InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERGAME_API UMyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float Health;

	UFUNCTION()
	void OnTakeDamage(AActor* DamageActor, float Damage, const class UDamageType * DamageType,
					class AController* InstigateBy, AActor* DamageCauser);

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChangedEvent;
};
