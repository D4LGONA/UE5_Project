// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AS_WeaponAttributes.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class DEUS_EX_ASH_API UAS_WeaponAttributes : public UAttributeSet
{
	GENERATED_BODY()
	
public:
    UAS_WeaponAttributes();

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    // 데미지
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(UAS_WeaponAttributes, Damage)

    // 내구도
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Durability;
    ATTRIBUTE_ACCESSORS(UAS_WeaponAttributes, Durability)

    // 최대 내구도
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData MaxDurability;
    ATTRIBUTE_ACCESSORS(UAS_WeaponAttributes, MaxDurability)
};
