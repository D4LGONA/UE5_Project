// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_WeaponAttributes.h"
#include "CharacterBase.h"
#include "WeaponBase.h"
#include "AbilitySystemUtility.h"
#include "GameplayEffectExtension.h"


UAS_WeaponAttributes::UAS_WeaponAttributes()
{
	Damage = 0.0f;
	Durability = 0.0f;
	MaxDurability = 100.0f;
}

void UAS_WeaponAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetDurabilityAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxDurability());
	}
}

void UAS_WeaponAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDurabilityAttribute())
	{
		SetDurability(GetDurability());

		AActor* WeaponActor = GetOwningActor();
		AWeaponBase* WeaponBase = Cast<AWeaponBase>(WeaponActor);

		AActor* WeaponOwnerActor = WeaponActor ? WeaponActor->GetOwner() : nullptr;
		ACharacterBase* WeaponOwnerCharacterBase = Cast<ACharacterBase>(WeaponOwnerActor);

		if (WeaponActor && WeaponBase && WeaponOwnerActor && WeaponOwnerCharacterBase)
		{
			// 무기 내구도가 0인 경우
			if (FMath::IsNearlyZero(GetDurability(), 1.0E-4F))
			{
				// 무기 부서짐 체크가 안되있으면
				if (!WeaponBase->bIsBroken)
				{
					switch (WeaponOwnerCharacterBase->CharacterType)
					{
						case ECharacterType::Player:

							break;

						case ECharacterType::Enemy:
							// 그로기 태그 전송
							FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::GroggyTag, WeaponOwnerActor, WeaponOwnerActor, 3.0f, WeaponOwnerCharacterBase->AbilitySystemComponent);
							break;

						default:
							break;
					}

					WeaponBase->bIsBroken = true;
				}
			}

			float CurrentDurabilityRatio = WeaponBase->AS_WeaponAttributes->GetDurability() / WeaponBase->AS_WeaponAttributes->GetMaxDurability();

			// 무기 내구도가 50% 미만인 경우 데미지 디버프
			if (CurrentDurabilityRatio < 0.5f)
			{
				if (WeaponBase->WeaponDamageInfiniteHandle.IsValid())
				{
					WeaponBase->AbilitySystemComponent->RemoveActiveGameplayEffect(WeaponBase->WeaponDamageInfiniteHandle);
				}

				float BaseWeaponDamage = WeaponBase->AS_WeaponAttributes->GetDamage();
				float WeaponDamageAdder = -(BaseWeaponDamage * (0.5 - CurrentDurabilityRatio));
				WeaponBase->WeaponDamageInfiniteHandle = FAbilitySystemUtility::Get().ApplyGameplayEffect(WeaponBase->AbilitySystemComponent, WeaponBase->AbilitySystemComponent, WeaponBase->GE_WeaponDamage_Infinite, 0.0f, FAbilitySystemUtility::WeaponDamageInfiniteDataTag, WeaponDamageAdder);
			}
			else
			{
				if (WeaponBase->WeaponDamageInfiniteHandle.IsValid())
				{
					WeaponBase->AbilitySystemComponent->RemoveActiveGameplayEffect(WeaponBase->WeaponDamageInfiniteHandle);
				}
			}
		}
		else
		{
			//
			// null
			//
		}
	}
}
