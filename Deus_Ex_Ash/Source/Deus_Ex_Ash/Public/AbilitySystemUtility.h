// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"

class DEUS_EX_ASH_API FAbilitySystemUtility
{
public:
	static FAbilitySystemUtility& Get();
	void SendEventTag(const FGameplayTag Tag, AActor* Instigator, AActor* Target, UAbilitySystemComponent* ASC);
	void SendEventTag(const FGameplayTag Tag, AActor* Instigator, AActor* Target, const float Magnitude, UAbilitySystemComponent* ASC);
	void SendEventTag(const FGameplayTag Tag, AActor* Instigator, AActor* Target, FGameplayTagContainer InstigatorTags, const float Magnitude, UAbilitySystemComponent* ASC);
	FActiveGameplayEffectHandle ApplyGameplayEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> Effect, const float Level, const FGameplayTag DataTag, const float Magnitude);
	
	// Initialization helper (call from module StartupModule() if you prefer eager init)
	static void InitializeTags();

	// Accessors (lazy init)
	static const FGameplayTag& GetGuardingTag();
	static const FGameplayTag& GetPerfectGuardingTag();
	static const FGameplayTag& GetGuardRegainTag();
	static const FGameplayTag& GetInvincibleTag();

	static const FGameplayTag& GetGuardSuccessTag();
	static const FGameplayTag& GetPerfectGuardSuccessTag();

	static const FGameplayTag& GetHitTag();
	static const FGameplayTag& GetHitForwardTag();
	static const FGameplayTag& GetHitBackwardTag();
	static const FGameplayTag& GetHitLeftwardTag();
	static const FGameplayTag& GetHitRightwardTag();
	static const FGameplayTag& GetGuardBreakHitTag();
	static const FGameplayTag& GetParryPushBackTag();
	static const FGameplayTag& GetGroggyTag();
	static const FGameplayTag& GetStaggerTag();
	static const FGameplayTag& GetDieTag();
	static const FGameplayTag& GetRespawnTag();
	static const FGameplayTag& GetKnockdownTag();

	static const FGameplayTag& GetWeaponDurabilityInstantDataTag();
	static const FGameplayTag& GetWeaponDamageInfiniteDataTag();
	static const FGameplayTag& GetStatHealthDataTag();
	static const FGameplayTag& GetStatTempHealthOverrideDataTag();
	static const FGameplayTag& GetStatTempHealthInstantDataTag();
	static const FGameplayTag& GetStatStaminaInstantDataTag();

public:
	FAbilitySystemUtility() {};
	FAbilitySystemUtility(const FAbilitySystemUtility&) = delete;
	FAbilitySystemUtility& operator=(const FAbilitySystemUtility&) = delete;

	// Backing storage (non-const so we can initialize later)
	static FGameplayTag GuardingTag;
	static FGameplayTag PerfectGuardingTag;
	static FGameplayTag GuardRegainTag;
	static FGameplayTag InvincibleTag;

	static FGameplayTag GuardSuccessTag;
	static FGameplayTag PerfectGuardSuccessTag;

	static FGameplayTag HitTag;
	static FGameplayTag HitForwardTag;
	static FGameplayTag HitBackwardTag;
	static FGameplayTag HitLeftwardTag;
	static FGameplayTag HitRightwardTag;
	static FGameplayTag GuardBreakHitTag;
	static FGameplayTag ParryPushBackTag;
	static FGameplayTag GroggyTag;
	static FGameplayTag StaggerTag;
	static FGameplayTag DieTag;
	static FGameplayTag RespawnTag;
	static FGameplayTag KnockdownTag;

	static FGameplayTag WeaponDurabilityInstantDataTag;
	static FGameplayTag WeaponDamageInfiniteDataTag;
	static FGameplayTag StatHealthDataTag;
	static FGameplayTag StatTempHealthOverrideDataTag;
	static FGameplayTag StatTempHealthInstantDataTag;
	static FGameplayTag StatStaminaInstantDataTag;
};
