// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystemUtility.h"

// Backing storage (no RequestGameplayTag here)
FGameplayTag FAbilitySystemUtility::GuardingTag;
FGameplayTag FAbilitySystemUtility::PerfectGuardingTag;
FGameplayTag FAbilitySystemUtility::GuardRegainTag;
FGameplayTag FAbilitySystemUtility::InvincibleTag;

FGameplayTag FAbilitySystemUtility::GuardSuccessTag;
FGameplayTag FAbilitySystemUtility::PerfectGuardSuccessTag;

FGameplayTag FAbilitySystemUtility::HitTag;
FGameplayTag FAbilitySystemUtility::HitForwardTag;
FGameplayTag FAbilitySystemUtility::HitBackwardTag;
FGameplayTag FAbilitySystemUtility::HitLeftwardTag;
FGameplayTag FAbilitySystemUtility::HitRightwardTag;
FGameplayTag FAbilitySystemUtility::GuardBreakHitTag;
FGameplayTag FAbilitySystemUtility::ParryPushBackTag;
FGameplayTag FAbilitySystemUtility::GroggyTag;
FGameplayTag FAbilitySystemUtility::StaggerTag;
FGameplayTag FAbilitySystemUtility::DieTag;
FGameplayTag FAbilitySystemUtility::RespawnTag;
FGameplayTag FAbilitySystemUtility::KnockdownTag;

FGameplayTag FAbilitySystemUtility::WeaponDurabilityInstantDataTag;
FGameplayTag FAbilitySystemUtility::WeaponDamageInfiniteDataTag;
FGameplayTag FAbilitySystemUtility::StatHealthDataTag;
FGameplayTag FAbilitySystemUtility::StatTempHealthOverrideDataTag;
FGameplayTag FAbilitySystemUtility::StatTempHealthInstantDataTag;
FGameplayTag FAbilitySystemUtility::StatStaminaInstantDataTag;

FAbilitySystemUtility& FAbilitySystemUtility::Get()
{
	static FAbilitySystemUtility Instance;
	return Instance;
}

void FAbilitySystemUtility::InitializeTags()
{
	// Safe to call once engine subsystems are initialized (e.g., from module StartupModule)
	GuardingTag = FGameplayTag::RequestGameplayTag(FName("State.Combat.Guarding"));
	PerfectGuardingTag = FGameplayTag::RequestGameplayTag(FName("State.Combat.PerfectGuarding"));
	GuardRegainTag = FGameplayTag::RequestGameplayTag(FName("State.Combat.GuardRegain"));
	InvincibleTag = FGameplayTag::RequestGameplayTag(FName("State.Invincible"));

	GuardSuccessTag = FGameplayTag::RequestGameplayTag(FName("Ability.Defense.GuardSuccess"));
	PerfectGuardSuccessTag = FGameplayTag::RequestGameplayTag(FName("Ability.Defense.PerfectGuardSuccess"));

	HitTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit"));
	HitForwardTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit.Forward"));
	HitBackwardTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit.Backward"));
	HitLeftwardTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit.Leftward"));
	HitRightwardTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit.Rightward"));
	GuardBreakHitTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.GuardBreakHit"));
	ParryPushBackTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.ParryPushBack"));
	GroggyTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Groggy"));
	StaggerTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Stagger"));
	DieTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Die"));
	RespawnTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Respawn"));
	KnockdownTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Knockdown"));

	WeaponDurabilityInstantDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Weapon.Durability.Instant"));
	WeaponDamageInfiniteDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Weapon.Damage.Infinite"));
	StatHealthDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.Health"));
	StatTempHealthOverrideDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.TempHealth.Override"));
	StatTempHealthInstantDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.TempHealth.Instant"));
	StatStaminaInstantDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.Stamina.Instant"));
}

// Lazy getters (safe if first call happens after engine is ready; otherwise prefer InitializeTags)
const FGameplayTag& FAbilitySystemUtility::GetGuardingTag()
{
	if (!GuardingTag.IsValid())
	{
		GuardingTag = FGameplayTag::RequestGameplayTag(FName("State.Combat.Guarding"));
	}
	return GuardingTag;
}

const FGameplayTag& FAbilitySystemUtility::GetPerfectGuardingTag()
{
	if (!PerfectGuardingTag.IsValid())
	{
		PerfectGuardingTag = FGameplayTag::RequestGameplayTag(FName("State.Combat.PerfectGuarding"));
	}
	return PerfectGuardingTag;
}

const FGameplayTag& FAbilitySystemUtility::GetGuardRegainTag()
{
	if (!GuardRegainTag.IsValid())
	{
		GuardRegainTag = FGameplayTag::RequestGameplayTag(FName("State.Combat.GuardRegain"));
	}
	return GuardRegainTag;
}

const FGameplayTag& FAbilitySystemUtility::GetInvincibleTag()
{
	if (!InvincibleTag.IsValid())
	{
		InvincibleTag = FGameplayTag::RequestGameplayTag(FName("State.Invincible"));
	}
	return InvincibleTag;
}

const FGameplayTag& FAbilitySystemUtility::GetGuardSuccessTag()
{
	if (!GuardSuccessTag.IsValid())
	{
		GuardSuccessTag = FGameplayTag::RequestGameplayTag(FName("Ability.Defense.GuardSuccess"));
	}
	return GuardSuccessTag;
}

const FGameplayTag& FAbilitySystemUtility::GetPerfectGuardSuccessTag()
{
	if (!PerfectGuardSuccessTag.IsValid())
	{
		PerfectGuardSuccessTag = FGameplayTag::RequestGameplayTag(FName("Ability.Defense.PerfectGuardSuccess"));
	}
	return PerfectGuardSuccessTag;
}

const FGameplayTag& FAbilitySystemUtility::GetHitTag()
{
	if (!HitTag.IsValid())
	{
		HitTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit"));
	}
	return HitTag;
}
const FGameplayTag& FAbilitySystemUtility::GetHitForwardTag()
{
	if (!HitForwardTag.IsValid())
	{
		HitForwardTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit.Forward"));
	}
	return HitForwardTag;
}
const FGameplayTag& FAbilitySystemUtility::GetHitBackwardTag()
{
	if (!HitBackwardTag.IsValid())
	{
		HitBackwardTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit.Backward"));
	}
	return HitBackwardTag;
}
const FGameplayTag& FAbilitySystemUtility::GetHitLeftwardTag()
{
	if (!HitLeftwardTag.IsValid())
	{
		HitLeftwardTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit.Leftward"));
	}
	return HitLeftwardTag;
}
const FGameplayTag& FAbilitySystemUtility::GetHitRightwardTag()
{
	if (!HitRightwardTag.IsValid())
	{
		HitRightwardTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Hit.Rightward"));
	}
	return HitRightwardTag;
}

const FGameplayTag& FAbilitySystemUtility::GetGuardBreakHitTag()
{
	if (!GuardBreakHitTag.IsValid())
	{
		GuardBreakHitTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.GuardBreakHit"));
	}
	return GuardBreakHitTag;
}

const FGameplayTag& FAbilitySystemUtility::GetParryPushBackTag()
{
	if (!ParryPushBackTag.IsValid())
	{
		ParryPushBackTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.ParryPushBack"));
	}
	return ParryPushBackTag;
}

const FGameplayTag& FAbilitySystemUtility::GetGroggyTag()
{
	if (!GroggyTag.IsValid())
	{
		GroggyTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Groggy"));
	}
	return GroggyTag;
}

const FGameplayTag& FAbilitySystemUtility::GetStaggerTag()
{
	if (!StaggerTag.IsValid())
	{
		StaggerTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Stagger"));
	}
	return StaggerTag;
}

const FGameplayTag& FAbilitySystemUtility::GetDieTag()
{
	if (!DieTag.IsValid())
	{
		DieTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Die"));
	}
	return DieTag;
}

const FGameplayTag& FAbilitySystemUtility::GetRespawnTag()
{
	if (!RespawnTag.IsValid())
	{
		RespawnTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Respawn"));
	}
	return RespawnTag;
}

const FGameplayTag& FAbilitySystemUtility::GetKnockdownTag()
{
	if (!KnockdownTag.IsValid())
	{
		KnockdownTag = FGameplayTag::RequestGameplayTag(FName("Ability.State.Knockdown"));
	}
	return KnockdownTag;
}

const FGameplayTag& FAbilitySystemUtility::GetWeaponDurabilityInstantDataTag()
{
	if (!WeaponDurabilityInstantDataTag.IsValid())
	{
		WeaponDurabilityInstantDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Weapon.Durability.Instant"));
	}
	return WeaponDurabilityInstantDataTag;
}

const FGameplayTag& FAbilitySystemUtility::GetWeaponDamageInfiniteDataTag()
{
	if (!WeaponDamageInfiniteDataTag.IsValid())
	{
		WeaponDamageInfiniteDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Weapon.Damage.Infinite"));
	}
	return WeaponDamageInfiniteDataTag;
}

const FGameplayTag& FAbilitySystemUtility::GetStatHealthDataTag()
{
	if (!StatHealthDataTag.IsValid())
	{
		StatHealthDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.Health"));
	}
	return StatHealthDataTag;
}

const FGameplayTag& FAbilitySystemUtility::GetStatTempHealthOverrideDataTag()
{
	if (!StatTempHealthOverrideDataTag.IsValid())
	{
		StatTempHealthOverrideDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.TempHealth.Override"));
	}
	return StatTempHealthOverrideDataTag;
}

const FGameplayTag& FAbilitySystemUtility::GetStatTempHealthInstantDataTag()
{
	if (!StatTempHealthInstantDataTag.IsValid())
	{
		StatTempHealthInstantDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.TempHealth.Instant"));
	}
	return StatTempHealthInstantDataTag;
}

const FGameplayTag& FAbilitySystemUtility::GetStatStaminaInstantDataTag()
{
	if (!StatStaminaInstantDataTag.IsValid())
	{
		StatStaminaInstantDataTag = FGameplayTag::RequestGameplayTag(FName("Effect.Data.Stat.Stamina.Instant"));
	}
	return StatStaminaInstantDataTag;
}

void FAbilitySystemUtility::SendEventTag(const FGameplayTag Tag, AActor* _Instigator, AActor* Target, UAbilitySystemComponent* ASC)
{
	FGameplayEventData EventData;
	EventData.EventTag = Tag;
	EventData.Instigator = _Instigator;
	EventData.Target = Target;
	ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
}

void FAbilitySystemUtility::SendEventTag(const FGameplayTag Tag, AActor* Instigator, AActor* Target, const float Magnitude, UAbilitySystemComponent* ASC)
{
	FGameplayEventData EventData;
	EventData.EventTag = Tag;
	EventData.Instigator = Instigator;
	EventData.Target = Target;
	EventData.EventMagnitude = Magnitude;
	ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
}

void FAbilitySystemUtility::SendEventTag(const FGameplayTag Tag, AActor* Instigator, AActor* Target, FGameplayTagContainer InstigatorTags, const float Magnitude, UAbilitySystemComponent* ASC)
{
	FGameplayEventData EventData;
	EventData.EventTag = Tag;
	EventData.Instigator = Instigator;
	EventData.Target = Target;
	EventData.InstigatorTags = InstigatorTags;
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
