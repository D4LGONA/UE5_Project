// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"

class DEUS_EX_ASH_API FAbilitySystemUtility
{
public:
	static FAbilitySystemUtility& Get();
	void SendEventTag(const FGameplayTag Tag, AActor* Instigator, AActor* Target, UAbilitySystemComponent* ASC);
	void SendEventTag(const FGameplayTag Tag, AActor* _Instigator, AActor* Target, const float Magnitude, UAbilitySystemComponent* ASC);
	FActiveGameplayEffectHandle ApplyGameplayEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> Effect, const float Level, const FGameplayTag DataTag, const float Magnitude);
	
private:
	FAbilitySystemUtility() {};
	FAbilitySystemUtility(const FAbilitySystemUtility&) = delete;
	FAbilitySystemUtility& operator=(const FAbilitySystemUtility&) = delete;

public:
	// Send Event Tag
	static const FGameplayTag GuardingTag;
	static const FGameplayTag PerfectGuardingTag;
	static const FGameplayTag GuardRegainTag;
	static const FGameplayTag GuardSuccessTag;
	static const FGameplayTag PerfectGuardSuccessTag;
	static const FGameplayTag HitTag;
	static const FGameplayTag GuardBreakHitTag;
	static const FGameplayTag ParryPushBackTag;
	static const FGameplayTag GroggyTag;
	static const FGameplayTag StaggerTag;
	static const FGameplayTag DieTag;
	static const FGameplayTag RespawnTag;
	static const FGameplayTag KnockdownTag;

	// Effect Data Tag
	static const FGameplayTag WeaponDurabilityInstantDataTag;
	static const FGameplayTag WeaponDamageInfiniteDataTag;
	static const FGameplayTag StatHealthDataTag;
	static const FGameplayTag StatTempHealthOverrideDataTag;
	static const FGameplayTag StatTempHealthInstantDataTag;
	static const FGameplayTag StatStaminaInstantDataTag;
};
