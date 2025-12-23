// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileBase.generated.h"


UCLASS()
class DEUS_EX_ASH_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly)
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovementComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ProjectileDamage;


	UFUNCTION(BlueprintCallable)
	void OnHit(AActor* HitActor, AActor* ProjectileActor, float Damage, float DurabilityDecrease, float StaggerDuration, bool IgnoreGuard);

	void SetFireDirection(const FVector& Direction);
};

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectileBase> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
};