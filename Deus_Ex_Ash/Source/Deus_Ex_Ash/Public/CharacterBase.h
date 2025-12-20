// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameItemData.h"
#include "GameSkillData.h"
#include "CharacterBase.generated.h"

class UAS_CharacterAttributes;
class AWeaponBase;

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	None UMETA(DisplayName = "None"),
	Player UMETA(DisplayName = "Player"),
	Enemy UMETA(DisplayName = "Enemy")
};

UENUM(BlueprintType)
enum class ECharacterDirection : uint8
{
	Forward UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

UCLASS()
class DEUS_EX_ASH_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	class UAS_CharacterAttributes* AS_CharacterAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> GE_Health;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> GE_TempHealth_Override;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> GE_TempHealth_Instant;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> GE_Stamina_Instant;


	// 스태미나 소모 행동 시 필요한 스태미나 최솟값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinStaminaRequired;

	// 플레이어가 적을 공격했는데 적이 방어했고 플레이어 스태미나 0일때 그로기 시간 -> 보류
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float GroggyTime_PlayerNoStaminaAndEnemyGuarded;

	// 플레이어가 저스트 가드 했는데 스태미나 0되서 걸리는 그로기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GroggyTime_PlayerJustGuardedAndNoStamina;

	// 저스트 가드 성공 시 스태미나 추가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaAdd_JustGuard;

	// 퍼펙트 가드 성공 시 스태미나 추가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaAdd_PerfectGuard;


	// 저스트 가드 성공 시 무기 내구도 추가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponDurabilityAdd_JustGuard;

	// 퍼펙트 가드 성공 시 무기 내구도 추가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponDurabilityAdd_PerfectGuard;

	// 퍼펙트 가드 성공 시 상대 스태미나 추가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackerStaminaAdd_PerfectGuard;

	// 퍼펙트 가드 성공 시 상대 무기 내구도 추가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackerWeaponDurabilityAdd_PerfectGuard;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterType CharacterType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	AActor* EquippedWeapon;


	void ApplyDamage(AActor* AttackerCharacter, AActor* Projectile, FVector ImpactPoint, float Damage, float StaggerDuration, bool IgnoreGuard);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();
	virtual void Die_Implementation();

	FTimerHandle RemoveGuardRegainTimerHandle;
	void SetGuardRegainActive(bool Active, float TargetTempHealth);


private:
	void Hit(ACharacterBase* AttackerCharacterBase, ECharacterDirection HitDirection, float Damage, float StaggerDuration, bool IgnoreGuard);
};
