// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExeCalc_Stamina_Sprint.generated.h"

UCLASS()
class DEUS_EX_ASH_API UExeCalc_Stamina_Sprint : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

protected:
	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
