// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CutsceneData.generated.h"

UENUM(BlueprintType)
enum class ECutsceneKind : uint8 { Dialogue, Event };

UENUM(BlueprintType)
enum class EStepType : uint8 { ShowBG, ShowChar, HideChar, TextLine, Choice, Wait };

USTRUCT(BlueprintType)
struct FCutsceneChoice {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Label;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EffectTag;
};

USTRUCT(BlueprintType)
struct FCutsceneStep {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EStepType Type = EStepType::TextLine;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* Background = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Speaker;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true)) FText Text;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FCutsceneChoice> Choices;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float WaitSeconds = 0.f;
};

UCLASS()
class PROJECT_API UCutsceneData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) ECutsceneKind Kind = ECutsceneKind::Dialogue;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FCutsceneStep> Steps;
};
