// Fill out your copyright notice in the Description page of Project Settings.


#include "ExeCalc_Stamina.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemUtility.h"
#include "CharacterBase.h"
#include "WeaponBase.h"
#include "AS_WeaponAttributes.h"
#include "AS_CharacterAttributes.h"

void UExeCalc_Stamina::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetOwner() : nullptr;

	ACharacterBase* TargetCharacterBase = Cast<ACharacterBase>(TargetActor);
	AWeaponBase* TargetWeaponBase = Cast<AWeaponBase>(TargetCharacterBase->EquippedWeapon_L);

	float BaseStamina = Spec.GetSetByCallerMagnitude(FAbilitySystemUtility::StatStaminaInstantDataTag);
	float FinalStamina = 0.0f;

	if (TargetActor && TargetCharacterBase && TargetWeaponBase)
	{
		float CurrentDurabilityRatio = TargetWeaponBase->AS_WeaponAttributes->GetDurability() / TargetWeaponBase->AS_WeaponAttributes->GetMaxDurability();
		if (CurrentDurabilityRatio < 0.5f)
		{
			FinalStamina = BaseStamina * (0.5 - CurrentDurabilityRatio);
		}

		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				TargetCharacterBase->AS_CharacterAttributes->GetStaminaAttribute(),
				EGameplayModOp::Additive,
				FinalStamina
			)
		);
	}
}