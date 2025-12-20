// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameItemData.h"
#include "GameSkillData.h"
#include "PlayerBase.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEUS_EX_ASH_API UPlayerBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerBase();

protected:
	virtual void BeginPlay() override;

public:
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Die();

	// 아이템 슬롯 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemSlot> ItemSlotList;

	// 스킬 슬롯 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillSlot> SkillSlotList;
};
