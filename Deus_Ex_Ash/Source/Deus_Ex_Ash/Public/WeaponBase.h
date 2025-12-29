// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "WeaponBase.generated.h"

class UAS_WeaponAttributes;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	Katana UMETA(DisplayName = "Katana")
};

UCLASS()
class DEUS_EX_ASH_API AWeaponBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CapsuleComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAS_WeaponAttributes* AS_WeaponAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> GE_WeaponDurability_Instant;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> GE_WeaponDamage_Infinite;
	FActiveGameplayEffectHandle WeaponDamageInfiniteHandle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(BlueprintReadWrite)
	int32 ComboIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBroken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GuardRegainHealAmount;


	UFUNCTION(BlueprintCallable)
	void OnAttackHit(AActor* HitActor, float Damage, float DurabilityAddAmount, float HitDuration, float StaggerDuration, bool IgnoreGuard);

private:
	TSet<AActor*> HitActors;

	UFUNCTION(BlueprintCallable)
	void AddHitActor(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool ContainsHitActor(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void ResetHitActors();
};
