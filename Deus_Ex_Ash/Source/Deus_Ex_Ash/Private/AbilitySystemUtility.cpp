// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystemUtility.h"

// Send Event Tag
const FGameplayTag FAbilitySystemUtility::GuardingTag = FGameplayTag::RequestGameplayTag(FName("State.Combat.Guarding"));
const FGameplayTag FAbilitySystemUtility::PerfectGuardingTag = FGameplayTag::RequestGameplayTag(FName("State.Combat.PerfectGuarding"));
const FGameplayTag FAbilitySystemUtility::GuardRegainTag = FGameplayTag::RequestGameplayTag(FName("State.Combat.GuardRegain"));
const FGameplayTag FAbilitySystemUtility::GuardSuccessTag = FGameplayTag::RequestGameplayTag(FName("Ability.Defense.GuardSuccess"));
const FGameplayTag FAbilitySystemUtility::PerfectGuardSuccessTag = FGameplayTag::RequestGameplayTag(FName("Ability.Defense.PerfectGuardSuccess"));
const FGameplayTag FAbilitySystemUtility::HitTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit"));
const FGameplayTag FAbilitySystemUtility::GuardBreakHitTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.GuardBreakHit"));
const FGameplayTag FAbilitySystemUtility::ParryPushBackTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.ParryPushBack"));
const FGameplayTag FAbilitySystemUtility::GroggyTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Groggy"));
const FGameplayTag FAbilitySystemUtility::StaggerTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Stagger"));
const FGameplayTag FAbilitySystemUtility::DieTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Die"));
const FGameplayTag FAbilitySystemUtility::RespawnTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Respawn"));
const FGameplayTag FAbilitySystemUtility::KnockdownTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Knockdown"));

// Effect Data Tag
const FGameplayTag FAbilitySystemUtility::WeaponDurabilityInstantDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Weapon.Durability.Instant"));
const FGameplayTag FAbilitySystemUtility::WeaponDamageInfiniteDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Weapon.Damage.Infinite"));
const FGameplayTag FAbilitySystemUtility::StatHealthDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.Health"));
const FGameplayTag FAbilitySystemUtility::StatTempHealthOverrideDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.TempHealth.Override"));
const FGameplayTag FAbilitySystemUtility::StatTempHealthInstantDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.TempHealth.Instant"));
const FGameplayTag FAbilitySystemUtility::StatStaminaInstantDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.Stamina.Instant"));

FAbilitySystemUtility& FAbilitySystemUtility::Get()
{
	static FAbilitySystemUtility Instance;
	return Instance;
}

void FAbilitySystemUtility::SendEventTag(const FGameplayTag Tag, AActor* _Instigator, AActor* Target, UAbilitySystemComponent* ASC)
{
	FGameplayEventData EventData;
	EventData.EventTag = Tag;
	EventData.Instigator = _Instigator;
	EventData.Target = Target;
	ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
}

void FAbilitySystemUtility::SendEventTag(const FGameplayTag Tag, AActor* _Instigator, AActor* Target, const float Magnitude, UAbilitySystemComponent* ASC)
{
	FGameplayEventData EventData;
	EventData.EventTag = Tag;
	EventData.Instigator = _Instigator;
	EventData.Target = Target;
	EventData.EventMagnitude = Magnitude;
	ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
}

FActiveGameplayEffectHandle FAbilitySystemUtility::ApplyGameplayEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> Effect, const float Level, const FGameplayTag DataTag, const float Magnitude)
{
	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(Effect, Level, Context);
	SpecHandle.Data->SetSetByCallerMagnitude(DataTag, Magnitude);
	return TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
