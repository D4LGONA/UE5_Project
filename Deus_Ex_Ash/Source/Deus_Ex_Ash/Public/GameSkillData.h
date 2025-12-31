// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "GameSkillData.generated.h"


USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefaultRemainingUses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag OnKeyPressTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EffectCooldownGrantedTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude;
};

USTRUCT(BlueprintType)
struct FSkillSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillData SkillData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAbilitySpecHandle AbilitySpecHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RemainingUses;
};


UCLASS()
class DEUS_EX_ASH_API UGameSkillData : public UObject
{
	GENERATED_BODY()
	
};
