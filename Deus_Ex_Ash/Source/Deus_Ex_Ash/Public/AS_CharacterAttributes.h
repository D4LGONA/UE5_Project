// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AS_CharacterAttributes.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class DEUS_EX_ASH_API UAS_CharacterAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAS_CharacterAttributes();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:
	// 체력
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributes, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributes, MaxHealth)

	// 임시 체력 (가드 리게인)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData TempHealth;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributes, TempHealth)

	// 스태미나
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributes, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributes, MaxStamina)


	// 기본 이동속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Attributes")
	FGameplayAttributeData DefaultMovementSpeed;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributes, DefaultMovementSpeed)

	// 달리기 속도 배율
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Attributes")
	FGameplayAttributeData SprintSpeedMultipler;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributes, SprintSpeedMultipler)

	// 대쉬 속도 배율
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Attributes")
	FGameplayAttributeData DashSpeedMultipler;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributes, DashSpeedMultipler)
};
