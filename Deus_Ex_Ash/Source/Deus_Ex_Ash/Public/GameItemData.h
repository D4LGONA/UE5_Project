// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/Texture2D.h"
#include "GameItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefaultRemainingUses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag OnKeyPressTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag OnKeyReleaseTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude;
};

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RemainingUses;
};

UCLASS()
class DEUS_EX_ASH_API UGameItemData : public UObject
{
	GENERATED_BODY()
	
};
