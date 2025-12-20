// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_CharacterAttributes.h"
#include "GameplayEffectExtension.h"
#include "CharacterBase.h"
#include "AbilitySystemUtility.h"

UAS_CharacterAttributes::UAS_CharacterAttributes()
{
	Health = 100.0f;
	MaxHealth = 100.0f;

	Stamina = 100.0f;
	MaxStamina = 100.0f;

	DefaultMovementSpeed = 500.0f;
	SprintSpeedMultipler = 1.7f;
	DashSpeedMultipler = 2.0f;
}

void UAS_CharacterAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}

void UAS_CharacterAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());

		AActor* CharacterActor = GetOwningActor();
		ACharacterBase* CharacterBase = Cast<ACharacterBase>(CharacterActor);
		
		if (CharacterActor && CharacterBase)
		{
			// 체력이 0인 경우
			if (FMath::IsNearlyZero(GetHealth(), 1.0E-4F))
			{
				CharacterBase->Die();
			}
			else
			{
				// 가드 리게인 태그 보유중이면
				if (CharacterBase->AbilitySystemComponent->HasMatchingGameplayTag(FAbilitySystemUtility::GuardRegainTag))
				{
					// 현재 체력이 임시체력보다 같거나 더 큰 경우
					if (GetHealth() > GetTempHealth() || FMath::IsNearlyEqual(GetTempHealth(), GetHealth(), 1.0E-4F))
					{
						// 가드 리게인 비활성화
						CharacterBase->SetGuardRegainActive(false, 0.0f);
					}
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
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(GetStamina());

		AActor* CharacterActor = GetOwningActor();
		ACharacterBase* CharacterBase = Cast<ACharacterBase>(CharacterActor);

		if (CharacterActor && CharacterBase)
		{
			// 스태미나가 0인 경우
			if (FMath::IsNearlyZero(GetStamina(), 1.0E-4F))
			{
				switch (CharacterBase->CharacterType)
				{
				case ECharacterType::Player:

					break;

				case ECharacterType::Enemy:
					// 다음 프레임에 그로기 태그 전송 (다른 어빌리티와 동시 실행 방지)
					GetWorld()->GetTimerManager().SetTimerForNextTick(
						[CharacterActor, CharacterBase]() {
							FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::GroggyTag, CharacterActor, CharacterActor, 3.0f, CharacterBase->AbilitySystemComponent);
						}
					);
					break;

				default:
					break;
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