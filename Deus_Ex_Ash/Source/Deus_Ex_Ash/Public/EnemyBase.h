// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyBase.generated.h"


UCLASS(ClassGroup = (Custom), meta=(BlueprintSpawnableComponent))
class DEUS_EX_ASH_API UEnemyBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyBase();

protected:
	virtual void BeginPlay() override;

public:
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Die();
};
